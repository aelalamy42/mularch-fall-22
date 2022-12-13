/*
============================================================================
Filename    : algorithm.c
Author      : Elalamy Balducci
SCIPER      : ______ - 325035
============================================================================
*/

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <cuda_runtime.h>
using namespace std;

// CPU Baseline
void array_process(double *input, double *output, int length, int iterations)
{
    double *temp;

    for(int n=0; n<(int) iterations; n++)
    {
        for(int i=1; i<length-1; i++)
        {
            for(int j=1; j<length-1; j++)
            {
                output[(i)*(length)+(j)] = (input[(i-1)*(length)+(j-1)] +
                                            input[(i-1)*(length)+(j)]   +
                                            input[(i-1)*(length)+(j+1)] +
                                            input[(i)*(length)+(j-1)]   +
                                            input[(i)*(length)+(j)]     +
                                            input[(i)*(length)+(j+1)]   +
                                            input[(i+1)*(length)+(j-1)] +
                                            input[(i+1)*(length)+(j)]   +
                                            input[(i+1)*(length)+(j+1)] ) / 9;

            }
        }
        output[(length/2-1)*length+(length/2-1)] = 1000;
        output[(length/2)*length+(length/2-1)]   = 1000;
        output[(length/2-1)*length+(length/2)]   = 1000;
        output[(length/2)*length+(length/2)]     = 1000;

        temp = input;
        input = output;
        output = temp;
    }
}

__global__
void single_iteration(double* input, double* output, int length){
    int j = blockIdx.x * blockDim.x + threadIdx.x + 1;
    int i = blockIdx.y * blockDim.y + threadIdx.y + 1;
    // Offset of one not to touch the border of the matrix
    // As we only have length - 2 threads in each dimension, the max is by definition length - 1
    output[(i)*(length)+(j)] = (input[(i-1)*(length)+(j-1)] +
                                        input[(i-1)*(length)+(j)]   +
                                        input[(i-1)*(length)+(j+1)] +
                                        input[(i)*(length)+(j-1)]   +
                                        input[(i)*(length)+(j)]     +
                                        input[(i)*(length)+(j+1)]   +
                                        input[(i+1)*(length)+(j-1)] +
                                        input[(i+1)*(length)+(j)]   +
                                        input[(i+1)*(length)+(j+1)] ) / 9;
}

__global__
void init_center(double* input, int length){
    input[(length/2-1)*length+(length/2-1)] = 1000;
    input[(length/2)*length+(length/2-1)]   = 1000;
    input[(length/2-1)*length+(length/2)]   = 1000;
    input[(length/2)*length+(length/2)]     = 1000;
}
// GPU Optimized function
void GPU_array_process(double *input, double *output, int length, int iterations)
{
    //Cuda events for calculating elapsed time
    cudaEvent_t cpy_H2D_start, cpy_H2D_end, comp_start, comp_end, cpy_D2H_start, cpy_D2H_end;
    cudaEventCreate(&cpy_H2D_start);
    cudaEventCreate(&cpy_H2D_end);
    cudaEventCreate(&cpy_D2H_start);
    cudaEventCreate(&cpy_D2H_end);
    cudaEventCreate(&comp_start);
    cudaEventCreate(&comp_end);

    /* Preprocessing goes here */
    //Alloc Space in GPU
    int size = length * length * sizeof(double);
    int threadsPerBlockSide = 8;
    double blocksPerGridSide = ceil((length - 2) / threadsPerBlockSide);
    dim3 threadsPerBlock(threadsPerBlockSide, threadsPerBlockSide);
    dim3 nbBlocks(blocksPerGridSide, blocksPerGridSide);
    double* d_input;
    cudaMalloc(&d_input, size);
    double* d_output;
    cudaMalloc(&d_output, size);
    cudaEventRecord(cpy_H2D_start);
    /* Copying array from host to device goes here */
    //Copy the array to the GPU
    cudaMemcpy(d_input, input, size, cudaMemcpyHostToDevice);
    cudaEventRecord(cpy_H2D_end);
    cudaEventSynchronize(cpy_H2D_end);

    //Copy array from host to device
    cudaEventRecord(comp_start);
    /* GPU calculation goes here */
    double *temp;
    cudaDeviceSynchronize();
    for(int n=0; n<(int) iterations; n++)
    {
        single_iteration<<<nbBlocks,threadsPerBlock>>>(d_input, d_output, length);
        init_center<<<1,1>>>(d_output, length);
        cudaDeviceSynchronize();
        temp = d_input;
        d_input = d_output;
        d_output = temp;
    }
    cudaEventRecord(comp_end);
    cudaEventSynchronize(comp_end);

    cudaEventRecord(cpy_D2H_start);
    /* Copying array from device to host goes here */
    //Copy result array in CPU
    cudaMemcpy(output, d_input, size, cudaMemcpyDeviceToHost); // The result is stored at the end of each iteration in the input array
    cudaEventRecord(cpy_D2H_end);
    cudaEventSynchronize(cpy_D2H_end);

    /* Postprocessing goes here */
    cudaFree(d_input);
    cudaFree(d_output);

    float time;
    cudaEventElapsedTime(&time, cpy_H2D_start, cpy_H2D_end);
    cout<<"Host to Device MemCpy takes "<<setprecision(4)<<time/1000<<"s"<<endl;

    cudaEventElapsedTime(&time, comp_start, comp_end);
    cout<<"Computation takes "<<setprecision(4)<<time/1000<<"s"<<endl;

    cudaEventElapsedTime(&time, cpy_D2H_start, cpy_D2H_end);
    cout<<"Device to Host MemCpy takes "<<setprecision(4)<<time/1000<<"s"<<endl;
}