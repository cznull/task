#include "cal.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_gl_interop.h>
#include <cudnn.h>

#define Dt 0.000005f

float *act[8] = { NULL };
float *para[6] = { NULL };
float *grad[4] = { NULL };
unsigned char *data = NULL;
float *buffer = NULL;
float *bufferd = NULL;
int n;
cudaGraphicsResource *cuda_vbo_resource;
cudnnHandle_t dnnh;
cudnnActivationDescriptor_t dnnad;
cudnnTensorDescriptor_t  dnntd[9];
cudnnFilterDescriptor_t dnnfd[3];
cudnnConvolutionDescriptor_t dnncd[2];
cudnnConvolutionFwdAlgo_t dnnalgo[3];
cudnnPoolingDescriptor_t dnnpd;

int cudainit(HWND hWnd) {
	cudaError_t cudaStatus;
	cudnnStatus_t  cudnnStatus;
	int i, j, k, l;
	float x;
	float alpha, beta;
	alpha = 1.0;
	beta = 0.0;
	FILE *fi;

	cudaStatus = cudaSetDevice(0);
	cudaStatus = cudaMalloc(act, (28 * 28 + 8 * 28 * 28 + 8 * 28 * 28 + 8 * 14 * 14 + 16 * 14 * 14 + 16 * 14 * 14 + 16 * 7 * 7 + 10) * sizeof(float) * 128);
	act[1] = act[0] + 128 * 28 * 28;
	act[2] = act[1] + 128 * 8 * 28 * 28;
	act[3] = act[2] + 128 * 8 * 28 * 28;
	act[4] = act[3] + 128 * 8 * 14 * 14;
	act[5] = act[4] + 128 * 16 * 14 * 14;
	act[6] = act[5] + 128 * 16 * 14 * 14;
	act[7] = act[6] + 128 * 16 * 7 * 7;
	cudaStatus = cudaMalloc(para, (8 * 5 * 5 + 8 * 1 * 1 + 16 * 8 * 5 * 5 + 16 * 1 * 1 + 10 * 16 * 7 * 7 + 10) * sizeof(float));
	para[1] = para[0] + 8 * 5 * 5;
	para[2] = para[1] + 8 * 1 * 1;
	para[3] = para[2] + 16 * 8 * 5 * 5;
	para[4] = para[3] + 16 * 1 * 1;
	para[5] = para[4] + 10 * 16 * 7 * 7;
	cudaStatus = cudaMalloc(grad, (8 * 28 * 28 + 8 * 14 * 14 + 16 * 14 * 14 + 16 * 7 * 7) * 128 * sizeof(float));
	grad[1] = grad[0] + 128 * 8 * 28 * 28;
	grad[2] = grad[1] + 128 * 8 * 14 * 14;
	grad[3] = grad[2] + 128 * 16 * 14 * 14;
	cudaStatus = cudaMalloc(&bufferd, (28 * 28 + 10) * 60000 * sizeof(float));


	data = (unsigned char*)malloc((28 * 28 * 60000 + 60000) * sizeof(unsigned char));
	if (!fopen_s(&fi, "D:/download/train-images.idx3-ubyte", "r")) {
		fseek(fi, 16, SEEK_SET);
		fread(data, 1, 28 * 28 * 60000, fi);
		fclose(fi);
	}
	if (!fopen_s(&fi, "D:/download/train-labels.idx1-ubyte", "r")) {
		fseek(fi, 8, SEEK_SET);
		fread(data + 28 * 28 * 60000, 1, 60000, fi);
		fclose(fi);
	}
	buffer = (float*)malloc((28 * 28 + 10) * 60000 * sizeof(float));
	for (i = 0; i < 28 * 28 * 60000; i++) {
		buffer[i] = (float)(data[i])/256.0f;
	}
	memset(buffer + 28 * 28 * 60000, 0, 60000 * 10 * sizeof(float));
	for (i = 0; i < 60000; i++) {
		buffer[28 * 28 * 60000 + i * 10 + data[28 * 28 * 60000 + i]] = 1.0f;
	}
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			for (k = 0; k < 28; k++) {
				for (l = 0; l < 28; l++) {
					data[((i * 28 + k) * 1024 + j * 28 + l) * 3] = buffer[(i * 32 + j) * 28 * 28 + k * 28 + l] * 256;
					data[((i * 28 + k) * 1024 + j * 28 + l) * 3 + 1] = buffer[(i * 32 + j) * 28 * 28 + k * 28 + l] * 256;
					data[((i * 28 + k) * 1024 + j * 28 + l) * 3 + 2] = buffer[(i * 32 + j) * 28 * 28 + k * 28 + l] * 256;
				}
			}
		}
	}
	cudaStatus = cudaMemcpy(bufferd, buffer, (28 * 28 + 10) * 60000 * sizeof(float), cudaMemcpyHostToDevice);
	if (!fopen_s(&fi, "D:/files/data/fig", "r")) {
		fread(buffer, sizeof(float), (8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10), fi);
		fclose(fi);
	}
	else {
		for (i = 0; i < 8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10; i++) {
			buffer[i] = (rand() / 32768.0f - 0.5f)*0.25f;
		}
		for (i = 0; i < 8 * 5 * 5; i++) {
			buffer[i] = (rand() / 32768.0f - 0.5f)*1.0f;
		}
	}
	cudaStatus = cudaMemcpy(para[0], buffer, (8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10) * sizeof(float), cudaMemcpyHostToDevice);

	cudnnStatus = cudnnCreate(&dnnh);
	cudnnStatus = cudnnCreateActivationDescriptor(&dnnad);
	cudnnStatus = cudnnSetActivationDescriptor(dnnad, CUDNN_ACTIVATION_RELU, CUDNN_NOT_PROPAGATE_NAN, 0.0);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 0);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 1);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 2);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 3);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 4);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 5);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 6);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 7);
	cudnnStatus = cudnnCreateTensorDescriptor(dnntd + 8);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[0], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 1, 28, 28);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[1], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 8, 28, 28);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[2], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 8, 14, 14);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[3], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 16, 14, 14);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[4], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 16, 7, 7);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[5], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 128, 10, 1, 1);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[6], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, 8, 1, 1);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[7], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, 16, 1, 1);
	cudnnStatus = cudnnSetTensor4dDescriptor(dnntd[8], CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, 10, 1, 1);
	cudnnStatus = cudnnCreateFilterDescriptor(dnnfd + 0);
	cudnnStatus = cudnnCreateFilterDescriptor(dnnfd + 1);
	cudnnStatus = cudnnCreateFilterDescriptor(dnnfd + 2);
	cudnnStatus = cudnnSetFilter4dDescriptor(dnnfd[0], CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, 8, 1, 5, 5);
	cudnnStatus = cudnnSetFilter4dDescriptor(dnnfd[1], CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, 16, 8, 5, 5);
	cudnnStatus = cudnnSetFilter4dDescriptor(dnnfd[2], CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, 10, 16, 7, 7);
	cudnnStatus = cudnnCreateConvolutionDescriptor(dnncd + 0);
	cudnnStatus = cudnnCreateConvolutionDescriptor(dnncd + 1);
	cudnnStatus = cudnnSetConvolution2dDescriptor(dnncd[0], 2, 2, 1, 1, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
	cudnnStatus = cudnnSetConvolution2dDescriptor(dnncd[1], 0, 0, 1, 1, 1, 1, CUDNN_CONVOLUTION, CUDNN_DATA_FLOAT);
	cudnnStatus = cudnnCreatePoolingDescriptor(&dnnpd);
	cudnnStatus = cudnnSetPooling2dDescriptor(dnnpd, CUDNN_POOLING_MAX, CUDNN_NOT_PROPAGATE_NAN, 2, 2, 0, 0, 2, 2);//AVERAGE_COUNT_INCLUDE_PADDING
	cudnnStatus = cudnnGetConvolutionForwardAlgorithm(dnnh, dnntd[0], dnnfd[0], dnncd[0], dnntd[1], CUDNN_CONVOLUTION_FWD_NO_WORKSPACE, 0, dnnalgo + 0);
	cudnnStatus = cudnnGetConvolutionForwardAlgorithm(dnnh, dnntd[2], dnnfd[1], dnncd[0], dnntd[3], CUDNN_CONVOLUTION_FWD_NO_WORKSPACE, 0, dnnalgo + 1);
	cudnnStatus = cudnnGetConvolutionForwardAlgorithm(dnnh, dnntd[4], dnnfd[2], dnncd[1], dnntd[5], CUDNN_CONVOLUTION_FWD_NO_WORKSPACE, 0, dnnalgo + 2);
	n = 0;
	return 0;
Error:
	cudaFree(act);
	cudaFree(para);
	return 1;
}

int cudacalc(float rate,float decay,float *loss) {
	cudaError cudaStatus;
	cudnnStatus_t  cudnnStatus;
	FILE *fi;
	float alpha, beta, alpha2;
	alpha = 1.0f;
	beta = 0.0f;
	alpha2 = -1.0f;
	*loss = 0.0f;
	int i, j, k, l;
	for (i = n * 10 + 0; i < n * 10 + 450; i++) {
		cudaStatus = cudaMemcpy(act[0], bufferd + 28 * 28 * 128 * i, (28 * 28 * 128) * sizeof(float), cudaMemcpyDeviceToDevice);
		cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[0], act[0], dnnfd[0], para[0], dnncd[0], dnnalgo[0], NULL, 0, &beta, dnntd[1], act[1]);
		cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[6], para[1], &alpha, dnntd[1], act[1]);
		cudnnStatus = cudnnActivationForward(dnnh, dnnad, &alpha, dnntd[1], act[1], &beta, dnntd[1], act[2]);
		cudnnStatus = cudnnPoolingForward(dnnh, dnnpd, &alpha, dnntd[1], act[2], &beta, dnntd[2], act[3]);
		cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[2], act[3], dnnfd[1], para[2], dnncd[0], dnnalgo[1], NULL, 0, &beta, dnntd[3], act[4]);
		cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[7], para[3], &alpha, dnntd[3], act[4]);
		cudnnStatus = cudnnActivationForward(dnnh, dnnad, &alpha, dnntd[3], act[4], &beta, dnntd[3], act[5]);
		cudnnStatus = cudnnPoolingForward(dnnh, dnnpd, &alpha, dnntd[3], act[5], &beta, dnntd[4], act[6]);
		cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[4], act[6], dnnfd[2], para[4], dnncd[1], dnnalgo[2], NULL, 0, &beta, dnntd[5], act[7]);
		cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[8], para[5], &alpha, dnntd[5], act[7]);
		cudnnStatus = cudnnSoftmaxForward(dnnh, CUDNN_SOFTMAX_ACCURATE, CUDNN_SOFTMAX_MODE_CHANNEL, &alpha, dnntd[5], act[7], &beta, dnntd[5], act[7]);
		cudnnStatus = cudnnAddTensor(dnnh, &alpha2, dnntd[5], bufferd + 28 * 28 * 60000 + i * 128 * 10, &alpha, dnntd[5], act[7]);
		cudnnStatus = cudnnConvolutionBackwardBias(dnnh, &rate, dnntd[5], act[7], &decay, dnntd[8], para[5]);
		cudnnStatus = cudnnConvolutionBackwardData(dnnh, &alpha, dnnfd[2], para[4], dnntd[5], act[7], dnncd[1], CUDNN_CONVOLUTION_BWD_DATA_ALGO_0, NULL, 0, &beta, dnntd[4], grad[3]);
		cudnnStatus = cudnnConvolutionBackwardFilter(dnnh, &rate, dnntd[4], act[6], dnntd[5], act[7], dnncd[1], CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0, NULL, 0, &decay, dnnfd[2], para[4]);
		cudnnStatus = cudnnPoolingBackward(dnnh, dnnpd, &alpha, dnntd[4], act[6], dnntd[4], grad[3], dnntd[3], act[5], &beta, dnntd[3], grad[2]);
		cudnnStatus = cudnnActivationBackward(dnnh, dnnad, &alpha, dnntd[3], act[5], dnntd[3], grad[2], dnntd[3], act[4], &beta, dnntd[3], grad[2]);
		cudnnStatus = cudnnConvolutionBackwardBias(dnnh, &rate, dnntd[3], grad[2], &decay, dnntd[7], para[3]);
		cudnnStatus = cudnnConvolutionBackwardData(dnnh, &alpha, dnnfd[1], para[2], dnntd[3], grad[2], dnncd[0], CUDNN_CONVOLUTION_BWD_DATA_ALGO_0, NULL, 0, &beta, dnntd[2], grad[1]);
		cudnnStatus = cudnnConvolutionBackwardFilter(dnnh, &rate, dnntd[2], act[3], dnntd[3], grad[2], dnncd[0], CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0, NULL, 0, &decay, dnnfd[1], para[2]);
		cudnnStatus = cudnnPoolingBackward(dnnh, dnnpd, &alpha, dnntd[2], act[3], dnntd[2], grad[1], dnntd[1], act[2], &beta, dnntd[1], grad[0]);
		cudnnStatus = cudnnActivationBackward(dnnh, dnnad, &alpha, dnntd[1], act[2], dnntd[1], grad[0], dnntd[1], act[1], &beta, dnntd[1], grad[0]);
		cudnnStatus = cudnnConvolutionBackwardBias(dnnh, &rate, dnntd[1], grad[0], &decay, dnntd[6], para[1]);
		cudnnStatus = cudnnConvolutionBackwardFilter(dnnh, &rate, dnntd[0], act[0], dnntd[1], grad[0], dnncd[0], CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0, NULL, 0, &decay, dnnfd[0], para[0]);
		//	cudaStatus = cudaMemcpy(buffer, para[0], (8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10) * sizeof(float), cudaMemcpyDeviceToHost);
	}
	n++;
	//if (n == 45)
		n = 0;
	cudaStatus = cudaMemcpy(act[0], bufferd + 28 * 28 * 128 * 450, (28 * 28 * 128) * sizeof(float), cudaMemcpyDeviceToDevice);
	cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[0], act[0], dnnfd[0], para[0], dnncd[0], dnnalgo[0], NULL, 0, &beta, dnntd[1], act[1]);
	cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[6], para[1], &alpha, dnntd[1], act[1]);
	cudnnStatus = cudnnActivationForward(dnnh, dnnad, &alpha, dnntd[1], act[1], &beta, dnntd[1], act[2]);
	cudnnStatus = cudnnPoolingForward(dnnh, dnnpd, &alpha, dnntd[1], act[2], &beta, dnntd[2], act[3]);
	cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[2], act[3], dnnfd[1], para[2], dnncd[0], dnnalgo[1], NULL, 0, &beta, dnntd[3], act[4]);
	cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[7], para[3], &alpha, dnntd[3], act[4]);
	cudnnStatus = cudnnActivationForward(dnnh, dnnad, &alpha, dnntd[3], act[4], &beta, dnntd[3], act[5]);
	cudnnStatus = cudnnPoolingForward(dnnh, dnnpd, &alpha, dnntd[3], act[5], &beta, dnntd[4], act[6]);
	cudnnStatus = cudnnConvolutionForward(dnnh, &alpha, dnntd[4], act[6], dnnfd[2], para[4], dnncd[1], dnnalgo[2], NULL, 0, &beta, dnntd[5], act[7]);
	cudnnStatus = cudnnAddTensor(dnnh, &alpha, dnntd[8], para[5], &alpha, dnntd[5], act[7]);
	cudnnStatus = cudnnSoftmaxForward(dnnh, CUDNN_SOFTMAX_ACCURATE, CUDNN_SOFTMAX_MODE_CHANNEL, &alpha, dnntd[5], act[7], &beta, dnntd[5], act[7]);
//	cudnnStatus = cudnnAddTensor(dnnh, &alpha2, dnntd[5], bufferd + 28 * 28 * 60000 + i * 128 * 10, &alpha, dnntd[5], act[7]);

	cudaStatus = cudaMemcpy(buffer, para[0], (8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10) * sizeof(float), cudaMemcpyDeviceToHost);
	if (!fopen_s(&fi, "D:/files/data/fig", "wb")) {
		fwrite(buffer, sizeof(float), (8 * 5 * 5 + 8 + 16 * 8 * 5 * 5 + 16 + 10 * 16 * 7 * 7 + 10), fi);
		fclose(fi);
	}
	cudaStatus = cudaMemcpy(buffer, act[7], (10) * 128 * sizeof(float), cudaMemcpyDeviceToHost);
	for (i = 0; i < 128 * 10; i++) {
		*loss -= buffer[28 * 28 * 60000 + 450 * 128 * 10 + i] * log10(buffer[i]);
	}
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			for (l = 0; l < 10; l++) {
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 0] = buffer[(i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 1] = buffer[(i * 4 + j / 8) * 10 + l] * 0;
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 2] = buffer[(i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 3] = buffer[(i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 4] = buffer[(i * 4 + j / 8) * 10 + l] * 0;
				data[((i * 30 + 1) * 1024 + j * 28 + l * 2) * 3 + 5] = buffer[(i * 4 + j / 8) * 10 + l] * 255;
			}
		}
	}
	cudaStatus = cudaMemcpy(buffer, act[2], (8*28*28) * 128 * sizeof(float), cudaMemcpyDeviceToHost);
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			for (k = 0; k < 28; k++) {
				for (l = 0; l < 28; l++) {
					data[((i * 30 + k + 2) * 1024 + j * 28 + l) * 3] = min(buffer[(i * 32 + j) * 28 * 28 + k * 28 + l], 2.0f) * 127;
					data[((i * 30 + k + 2) * 1024 + j * 28 + l) * 3 + 1] = min(buffer[(i * 32 + j) * 28 * 28 + k * 28 + l], 2.0f) * 127;
					data[((i * 30 + k + 2) * 1024 + j * 28 + l) * 3 + 2] = min(buffer[(i * 32 + j) * 28 * 28 + k * 28 + l], 2.0f) * 127;
				}
			}
		}
	}
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			for (l = 0; l < 10; l++) {
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 0] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 1] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 2] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 0;
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 3] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 4] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 255;
				data[((i * 30) * 1024 + j * 28 + l * 2) * 3 + 5] = buffer[28 * 28 * 60000 + 450 * 128 * 10 + (i * 4 + j / 8) * 10 + l] * 0;
			}
		}
	}
	if (cudaStatus != cudaSuccess || cudnnStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}

int cudafin(void) {
	cudaError cudaStatus;;
	cudnnDestroy(dnnh);
	cudaFree(act[0]);
	cudaFree(para[0]);
	free(data);
	free(buffer);
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		return 1;
	}
	return 0;
}