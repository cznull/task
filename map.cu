#include "stdafx.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

const int pointcount=32768;
const float3 Dt = { 0.01f,0.01f,0.01f };
float3 *device_p1 = NULL, *device_v1 = NULL, *device_a1 = NULL;
float *device_m = NULL;

__global__ void Kernelevo1(float3 *p, float3 *a, float *m) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
}
__global__ void Kernelevo2(float3 *p, float3 *v, float3 *a) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	v[x] += a[x] * Dt;
}

int cudainit(HWND hWnd) {
	cudaError_t cudaStatus;

	cudaStatus = cudaMalloc(&device_p1, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	cudaStatus = cudaMalloc(&device_v1, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	cudaStatus = cudaMalloc(&device_a1, pointcount * sizeof(float3));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	cudaStatus = cudaMalloc(&device_m, pointcount * sizeof(float));
	if (cudaStatus != cudaSuccess) {
		MessageBoxA(hWnd, "malloc failed", "message", MB_OK);
		goto Error;
	}

	return 0;
Error:
	cudaFree(device_p1);
	cudaFree(device_v1);
	cudaFree(device_a1);
	return 1;
}
