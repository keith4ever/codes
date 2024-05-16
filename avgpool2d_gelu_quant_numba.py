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

import os
import math
import torch
import torch.nn as nn
import numpy as np
from numba import cuda

@cuda.jit
def gpu_compute(kernel_size, stride, min_value, scale, input, output):
    idxC = cuda.threadIdx.x + cuda.blockIdx.x * cuda.blockDim.x
    idxY = cuda.threadIdx.y + cuda.blockIdx.y * cuda.blockDim.y
    idxX = cuda.threadIdx.z + cuda.blockIdx.z * cuda.blockDim.z
    if idxC >= output.shape[0] or idxY >= output.shape[1] or idxX >= output.shape[2]:
        return 

    sum:int = 0
    i = idxY * stride
    j = idxX * stride
    # AvgPool2d op
    for m in range(kernel_size):
        for n in range(kernel_size):
            if (i + m) >= input.shape[1] or (j + n) >= input.shape[2]:
                continue
            sum += input[idxC, i + m, j + n]

    # dequantize from int8 to float
    sum_dequant = (sum * scale) / (kernel_size * kernel_size) + min_value
    # GELU op
    output[idxC, idxY, idxX] = sum_dequant * (1.0 + math.erf(sum_dequant / math.sqrt(2.0))) / 2.0


class AvgPool2dGelu:
    def __init__(self, measure_error=True) -> None:
        self.kernel_size:int = 1
        self.stride:int = 1
        self.quant_error:bool = measure_error
        # self.padding = 0
        os.environ['NUMBAPRO_LIBDEVICE'] = "/usr/local/cuda/nvvm/libdevice"
        os.environ['NUMBAPRO_NVVM'] = "/usr/local/cuda/nvvm/lib64/libnvvm.so"

    def forward(self, input: torch.tensor, min_value, scale):
        channel = input.shape[0]
        # out_height = math.floor((input.shape[1] + 2 * self.padding - self.kernel_size) / self.stride + 1)
        # out_width = math.floor((input.shape[2] + 2 * self.padding - self.kernel_size) / self.stride + 1)
        out_height = math.floor((input.shape[1] - self.kernel_size) / self.stride + 1)
        out_width = math.floor((input.shape[2] - self.kernel_size) / self.stride + 1)
        
        d_input = cuda.to_device(input)
        d_output = cuda.device_array((channel, out_height, out_width), dtype=d_input.dtype) 

        threads_per_block = (4, 16, 16)
        bgrid_c = int((channel + threads_per_block[0] - 1) / threads_per_block[0])
        bgrid_y = int((out_height + threads_per_block[1] - 1) / threads_per_block[1])
        bgrid_x = int((out_width + threads_per_block[2] - 1) / threads_per_block[2])
        blocks_per_grid = (bgrid_c, bgrid_y, bgrid_x)

        gpu_compute[blocks_per_grid, threads_per_block]\
            (self.kernel_size, self.stride, min_value, scale, d_input, d_output)
        cuda.synchronize()
        return d_output.copy_to_host() 

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
        input_quant:np.int8 = np.rint((input - min_value)/scale)

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
    cuda.detect()
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
