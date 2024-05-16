# - Step 1:
# Implement a fused operation corresponding to average pool 2d (https://pytorch.org/docs/stable/generated/torch.nn.AvgPool2d.html)
# fused with a GELU activation (https://pytorch.org/docs/stable/generated/torch.nn.GELU.html)
# templated for float16, and float32, using Triton, OpenCL, CUDA, Metal, or any other GPU shading language of your choice.
# You may assume an input tensor of size [1, C,H, W], and limit the average pool 2d implementation to consider scalar values for kernel size and stride parameters; you may assume padding = 0. Please consider scalar values of 1, 2, and 3 for support.
# - Step 2:
# Add support for 8-bit input mapping to float16 or float32 output, with dequantization performed on-the-fly within the kernel. 8-bit quantization may beimplemented via a simple linear quantization scheme, applied per-channel. For a representative formula, see formula 12 in
# Quantization and Training of Neural Networks for Efficient Integer-Arithmetic-Only Inference (https://arxiv.org/pdf/1712.05877)
# - Step 3:
# Test your kernels for correctness. Provide discussion on how things may fail in production, guarding against client misuse, quantization scheme, or anything else you find interesting.

import math
import torch
import torch.nn as nn
import numpy as np
import pycuda.driver as cuda
import pycuda.autoinit
import pycuda.gpuarray as gpuarray
from pycuda.compiler import SourceModule

_kernel_str = """
    #include <math_constants.h>
                          
    __global__ void avgpool2d_gelu_dequant(
                          int kernel_size, int stride, 
                          float min_value, float scale, 
                          int ch, int in_height, int in_width, 
                          int height, int width,
                          unsigned char *input, float *output)
    {
        int idxC = threadIdx.x + blockIdx.x * blockDim.x; 
        int idxY = threadIdx.y + blockIdx.y * blockDim.y; 
        int idxX = threadIdx.z + blockIdx.z * blockDim.z; 
        if( idxC >= ch || idxY >= height || idxX >= width)
            return; 
      
        int sum = 0;
        int i = idxY * stride;
        int j = idxX * stride;
        // AvgPool2d op
        for(unsigned char m = 0; m < kernel_size; m++)
            for(unsigned char n = 0; n < kernel_size; n++){
                if((i + m) >= in_height || (j + n) >= in_width)
                    continue; 
                sum += input[idxC * in_height * in_width + (i + m)*in_width + j + n];
            }
        // dequantize from int8 to float, along with division by kernel_size ^ 2
        float sum_deq = float((sum * scale) / (kernel_size * kernel_size)) + min_value;
        // GELU op
        output[idxC * height * width + idxY * width + idxX] =
            0.5 * sum_deq * (1.0 + erff(sum_deq / sqrt(2.0)));
    }
"""

class AvgPool2dGelu:
    def __init__(self, measure_error=True) -> None:
        self.kernel_size:int = 1
        self.stride:int = 1
        self.quant_error:bool = measure_error
        # self.padding = 0

    def forward(self, input, min_value, scale):
        channel = input.shape[0]
        # out_height = math.floor((input.shape[1] + 2 * self.padding - self.kernel_size) / self.stride + 1)
        # out_width = math.floor((input.shape[2] + 2 * self.padding - self.kernel_size) / self.stride + 1)
        out_height = math.floor((input.shape[1] - self.kernel_size) / self.stride + 1)
        out_width = math.floor((input.shape[2] - self.kernel_size) / self.stride + 1)
        
        d_input = gpuarray.to_gpu(input)
        output = np.zeros([channel, out_height, out_width], dtype=np.float32)
        d_output = cuda.mem_alloc(output.nbytes)

        grid = (4, 16, 16)
        bgrid_c = int((channel + grid[0] - 1) / grid[0])
        bgrid_y = int((out_height + grid[1] - 1) / grid[1])
        bgrid_x = int((out_width + grid[2] - 1) / grid[2])
        block = (bgrid_c, bgrid_y, bgrid_x)

        gpu_module = SourceModule(_kernel_str)
        avgpool2d_gelu_dequant = gpu_module.get_function("avgpool2d_gelu_dequant")
        avgpool2d_gelu_dequant(np.int32(self.kernel_size), np.int32(self.stride), 
                    np.float32(min_value), np.float32(scale), 
                    np.int32(channel), np.int32(input.shape[1]), np.int32(input.shape[2]), 
                    np.int32(out_height), np.int32(out_width), d_input, d_output, 
                    block=block, grid=grid)
        cuda.memcpy_dtoh(output, d_output)
        return output

    def run(self, kernel_size, stride, channel, height, width, dtype) -> None:
        print(f'**** kernel={kernel_size}, stride={stride}, channel={channel}, \
        height={height}, width={width}, dtype: {dtype}')
        self.kernel_size = kernel_size
        self.stride = stride

        input = np.random.rand(channel, height, width).astype(dtype)

        # quant from fp16 or fp32 to int8
        max_value:dtype = input.max()
        min_value:dtype = input.min()
        scale = (max_value - min_value)/255
        input_quant = np.rint((input - min_value)/scale).astype(np.int8)

        output = self.forward(input_quant, min_value, scale)

        if self.quant_error:
        # get torch result as reference output
            input_pt = torch.from_numpy(input)
            avgpool2d_pt = nn.AvgPool2d(self.kernel_size, stride=self.stride, padding=0, \
                                        ceil_mode=False, count_include_pad=True)
            gelu_pt = nn.GELU()
            output_pt = avgpool2d_pt(input_pt)
            output_pt = gelu_pt(output_pt)

            # error from quantization in %: (output.fp - output.int8) * 100 / range of element value
            diff_perc = np.fabs(np.divide((output_pt - output) * 100, (max_value - min_value)))
            print("quant error: avg {:.3f}%, max {:.3f}%"\
                .format(np.average(diff_perc), diff_perc.max()))
            print(" ")


if __name__ == '__main__':
    model = AvgPool2dGelu()

    model.run(1, 1, 16, 50, 24, np.float32)
    model.run(2, 1, 8, 48, 32, np.float16)
    model.run(3, 1, 3, 66, 12, np.float16)
    model.run(1, 2, 1, 32, 52, np.float32)
    model.run(2, 2, 5, 98, 8, np.float32)
    model.run(3, 2, 11, 22, 15, np.float16)
    model.run(1, 3, 13, 34, 20, np.float32)
    model.run(2, 3, 7, 98, 64, np.float16)
    model.run(3, 3, 21, 12, 19, np.float32)
