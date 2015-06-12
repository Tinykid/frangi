///////////////////////////////////////////////////////////////////////////////
// convolution.h
// =============
///////////////////////////////////////////////////////////////////////////////

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

// 2D convolution (No Optimization) ///////////////////////////////////////////
// Simplest 2D convolution routine. It is easy to understand how convolution
// works, but is very slow, because of no optimization.
///////////////////////////////////////////////////////////////////////////////
bool convolve2DSlow(unsigned char* in, unsigned char* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);



// 2D convolution /////////////////////////////////////////////////////////////
// 2D data are usually stored as contiguous 1D array in computer memory.
// So, we are using 1D array for 2D data.
// 2D convolution assumes the kernel is center originated, which means, if
// kernel size 3 then, k[-1], k[0], k[1]. The middle of index is always 0.
// The following programming logics are somewhat complicated because of using
// pointer indexing in order to minimize the number of multiplications.
// It returns false if parameters are not valid.
///////////////////////////////////////////////////////////////////////////////
bool convolve2D(unsigned char* in, unsigned char* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);
bool convolve2D(unsigned short* in, unsigned short* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);
bool convolve2D(int* in, int* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);
bool convolve2D(float* in, float* out, int sizeX, int sizeY, float* kernel, int kSizeX, int kSizeY);
bool convolve2D(double* in, double* out, int sizeX, int sizeY, double* kernel, int kSizeX, int kSizeY);

#endif
