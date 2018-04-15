#include "ways.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_gl_interop.h>


int *device_map = NULL;
uchar3 *device_img = NULL;
struct cudaGraphicsResource *cuda_vbo_resource;
cudaError_t cudaStatus;

__global__ void fill(int *map, uchar3 *img, int w, int h, int imx) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x < w&&y < h) {
		uchar3 color;
		int i;
		i = map[x + y*w];
		color = img[x + y*imx];
		color.x = (i - 2) / 4 * 16;
		color.y = (i == 1) ? 255 : (color.y >> 1);
		color.z = (i >= 0) ? 255 : 0;
		img[x + y*imx] = color;
	}
}

int cudacalc(map* m, int imx) {
	dim3 b, g;
	b = { 16,16,1 };
	g = { (unsigned int(m->w) + 15) / 16,(unsigned int(m->h) + 15) / 16,1 };
	cudaStatus = cudaMemcpy(device_map, m->mapd, m->h*m->w * sizeof(int), cudaMemcpyHostToDevice);
	fill << <g, b >> > (device_map, device_img, m->w, m->h, imx);
	cudaStatus = cudaDeviceSynchronize();
	return 0;
}


int cudareg(GLuint pbo,map* m) {
	size_t num_bytes;
	if (device_map) {
		cudaStatus = cudaFree(device_map);
		if (cudaStatus != cudaSuccess) {
			return 1;
		}
	}
	cudaStatus = cudaMalloc(&device_map, m->h*m->w * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	cudaStatus = cudaGraphicsGLRegisterBuffer(&cuda_vbo_resource, pbo, cudaGraphicsMapFlagsWriteDiscard);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	cudaStatus = cudaGraphicsMapResources(1, &cuda_vbo_resource, 0);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	cudaStatus = cudaGraphicsResourceGetMappedPointer((void **)&device_img, &num_bytes, cuda_vbo_resource);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	cudaStatus = cudaGraphicsUnmapResources(1, &cuda_vbo_resource, 0);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}

int cudainit(void) {
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}

int cudafin(void) {
	cudaError cudaStatus;
	cudaFree(device_map);
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}
