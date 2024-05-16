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

