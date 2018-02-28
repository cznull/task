#include "map.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_gl_interop.h>

#define Dt 0.00003f

const int pointcount=32768,grsize=256,grcount= pointcount/grsize;
float3 *device_p1 = NULL, *device_v1 = NULL, *device_p2 = NULL;
float *device_m = NULL;
float *pattern = NULL;
struct cudaGraphicsResource *cuda_vbo_resource;

__global__ void evo1(float3 *p1, float3 *p2, float3 *v1, float *m) {
	__shared__ float temp[grsize * 4];
	int i, j, k;
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	float3 p0 = p1[x], a = { 0.0f,0.0f,0.0f }, r, *pt = (float3*)temp;
	float l2, l, *mt = temp + (grsize * 3);
	for (i = 0; i < grcount; i++) {
		k = threadIdx.x + i * blockDim.x;
		pt[threadIdx.x] = p1[k];
		mt[threadIdx.x] = m[k];
		__syncthreads();
		for (j = 0; j < grsize; j++) {
			r.x = pt[j].x - p0.x;
			r.y = pt[j].y - p0.y;
			r.z = pt[j].z - p0.z;
			l2 = r.x*r.x + r.y*r.y + r.z*r.z;
			l2 = fmaxf(l2, 0.00000001f);
			l2 = rsqrt(l2);
			l = mt[j] * l2*l2*l2;
			a.x += r.x*l;
			a.y += r.y*l;
			a.z += r.z*l;
		}
		__syncthreads();
	}
	r = v1[x];
	r.x += a.x*Dt;
	r.y += a.y*Dt;
	r.z += a.z*Dt;
	p0.x += r.x*Dt;
	p0.y += r.y*Dt;
	p0.z += r.z*Dt;
	p2[x] = p0;
	v1[x] = r;
}


int cudainit(HWND hWnd, GLuint vbo) {
	cudaError_t cudaStatus;
	size_t num_bytes;
	int i;
	float x;
	pattern = (float*)malloc(pointcount * 12);
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "device failed", "message", MB_OK);
		goto Error;
	
	}

	cudaStatus = cudaMalloc(&device_p2, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	/*cudaStatus = cudaMalloc(&device_p1, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}*/

	cudaStatus = cudaMalloc(&device_v1, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	cudaStatus = cudaMalloc(&device_m, pointcount * sizeof(float));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	cudaStatus = cudaGraphicsGLRegisterBuffer(&cuda_vbo_resource, vbo, cudaGraphicsMapFlagsWriteDiscard);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "res failed", "message", MB_OK);
		goto Error;
	}

	cudaGraphicsMapResources(1, &cuda_vbo_resource, 0);
	cudaStatus = cudaGraphicsResourceGetMappedPointer((void **)&device_p1, &num_bytes, cuda_vbo_resource);
	cudaGraphicsUnmapResources(1, &cuda_vbo_resource, 0);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "res failed", "message", MB_OK);
		goto Error;
	}

	for (i = 0; i < pointcount; i++) {
		pattern[i] = 1.0f;
	}
	cudaStatus = cudaMemcpy(device_m, pattern, pointcount * 4, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "copy failed", "message", MB_OK);
		goto Error;
	}
	for (i = 0; i < pointcount; i++) {
		pattern[i * 3] = sin(i*1.65452)*0.25f;
		pattern[i * 3 + 1] = sin(i*5.7678687)*0.25f;
		pattern[i * 3 + 2] = sin(i*3.56787)*0.25f;
	}
	

	cudaStatus = cudaMemcpy(device_p1, pattern, pointcount * 12, cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(device_p2, pattern, pointcount * 12, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "copy failed", "message", MB_OK);
		goto Error;
	}
	
	for (i = 0; i < pointcount; i++) {
		x = pattern[i * 3];
		pattern[i * 3] = sin(i*2.3466)*8.0f + pattern[i * 3 + 2] * 800.0f;
		pattern[i * 3 + 1] = sin(i*7.65452)*8.0f;
		pattern[i * 3 + 2] = sin(i*4.9741)*8.0f - x*800.0f;
	}

	cudaStatus = cudaMemcpy(device_v1, pattern, pointcount * 12, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "copy failed", "message", MB_OK);
		goto Error;
	}
	return 0;
Error:
	cudaFree(device_p2);
	cudaFree(device_v1);
	cudaFree(device_m);
	return 1;
}

int cudacalc(void) {
	cudaError cudaStatus;
	//cudaGraphicsMapResources(1, &cuda_vbo_resource, 0);
	evo1 << <pointcount / grsize, grsize >> > (device_p1, device_p2, device_v1, device_m);
	evo1 << <pointcount / grsize, grsize >> > (device_p2, device_p1, device_v1, device_m);
	cudaStatus = cudaDeviceSynchronize();
	//cudaGraphicsUnmapResources(1, &cuda_vbo_resource, 0);
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}

int cudafin(void) {
	cudaError cudaStatus;
	cudaFree(device_p2);
	cudaFree(device_v1);
	cudaFree(device_m);
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}
