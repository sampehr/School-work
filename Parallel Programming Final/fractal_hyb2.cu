#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <sys/time.h>
#include <cuda.h>

#define THREADSPERBLOCK 256

#define xMin 0.74395
#define xMax 0.74973
#define yMin 0.11321
#define yMax 0.11899


static void CudaTest(char *msg)
{
  cudaError_t e;

  cudaThreadSynchronize();
  if (cudaSuccess != (e = cudaGetLastError())) {
    fprintf(stderr, "%s: %d\n", msg, e);
    fprintf(stderr, "%s\n", cudaGetErrorString(e));
    exit(-1);
  }
}

#define mallocOnGPU(addr, size) if (cudaSuccess != cudaMalloc((void **)&addr, size)) fprintf(stderr, "could not allocate GPU memory\n");  CudaTest("couldn't allocate GPU memory");
#define copyFromGPU(to, from, size) if (cudaSuccess != cudaMemcpy(to, from, size, cudaMemcpyDeviceToHost)) fprintf(stderr, "copying of data from device failed\n");  CudaTest("data copy from device failed");

static __global__ void FractalKernel(int width, int from, int to, int maxdepth, double dx, double dy, unsigned char cnt[])
{  
  if((((threadIdx.x + blockIdx.x * blockDim.x) / width) + from) < to){	
	double cx, cy, x, y, x2, y2;
	int depth;
	
	int index = threadIdx.x + blockIdx.x * blockDim.x;
    int row = (index / width) + from;
	int col = index % width;
	
    cy = yMin + row * dy;
    cx = xMin + col * dx;
    x = -cx;
    y = -cy;
    depth = maxdepth;
    do {
      x2 = x * x;
      y2 = y * y;
      y = 2 * x * y - cy;
      x = x2 - y2 - cx;
      depth--;
    } while ((depth > 0) && ((x2 + y2) <= 5.0));
    cnt[(row - from) * width + col] = depth & 255;
  }
}

extern "C" unsigned char *GPU_Init(int size)
{
  // allocate array on GPU and return pointer to it
  unsigned char *cnt_device;
  mallocOnGPU(cnt_device, size);
  
  return cnt_device;
}

extern "C" void GPU_Exec(int width, int from, int to, int maxdepth, double dx, double dy, unsigned char *cnt_d)
{
    int blocks = ((to - from) * width + THREADSPERBLOCK - 1) / THREADSPERBLOCK;
    FractalKernel<<<blocks, THREADSPERBLOCK>>>(width, from, to, maxdepth, dx, dy, cnt_d);
}

extern "C" void GPU_Fini(unsigned char *cnt, unsigned char *cnt_d, int size)
{
  // copy the pixel data to the CPU and deallocate the GPU array
    copyFromGPU(cnt, cnt_d, size); //fourth argument for cudaMemcpy present above in #define copyFromGPU
    cudaFree(cnt_d);
}
