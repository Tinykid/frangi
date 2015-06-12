///////////////////////////////////////////////////////////////////////////////
// convolution.cpp
// ===============
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "convolution.h"

///////////////////////////////////////////////////////////////////////////////
// Simplest 2D convolution routine. It is easy to understand how convolution
// works, but is very slow, because of no optimization.
///////////////////////////////////////////////////////////////////////////////
bool convolve2DSlow(unsigned char* in, unsigned char* out, int dataSizeX, int dataSizeY,
                    float* kernel, int kernelSizeX, int kernelSizeY)
{
    int i, j, m, n, mm, nn;
    int kCenterX, kCenterY;                         // center index of kernel
    float sum;                                      // temp accumulation buffer
    int rowIndex, colIndex;

    // check validity of params
    if(!in || !out || !kernel) return false;
    if(dataSizeX <= 0 || kernelSizeX <= 0) return false;

    // find center position of kernel (half of kernel size)
    kCenterX = kernelSizeX / 2;
    kCenterY = kernelSizeY / 2;

    for(i=0; i < dataSizeY; ++i)                // rows
    {
        for(j=0; j < dataSizeX; ++j)            // columns
        {
            sum = 0;                            // init to 0 before sum
            for(m=0; m < kernelSizeY; ++m)      // kernel rows
            {
                mm = kernelSizeY - 1 - m;       // row index of flipped kernel

                for(n=0; n < kernelSizeX; ++n)  // kernel columns
                {
                    nn = kernelSizeX - 1 - n;   // column index of flipped kernel

                    // index of input signal, used for checking boundary
                    rowIndex = i + m - kCenterY;
                    colIndex = j + n - kCenterX;

                    // ignore input samples which are out of bound
                    if(rowIndex >= 0 && rowIndex < dataSizeY && colIndex >= 0 && colIndex < dataSizeX)
                        sum += in[dataSizeX * rowIndex + colIndex] * kernel[kernelSizeX * mm + nn];
                }
            }
            out[dataSizeX * i + j] = (unsigned char)((float)fabs(sum) + 0.5f);
        }
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// 2D convolution
// 2D data are usually stored in computer memory as contiguous 1D array.
// So, we are using 1D array for 2D data.
// 2D convolution assumes the kernel is center originated, which means, if
// kernel size 3 then, k[-1], k[0], k[1]. The middle of index is always 0.
// The following programming logics are somewhat complicated because of using
// pointer indexing in order to minimize the number of multiplications.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// unsigned char version (8bit): Note that the output is always positive number
///////////////////////////////////////////////////////////////////////////////
bool convolve2D(unsigned char* in, unsigned char* out, int dataSizeX, int dataSizeY, 
                float* kernel, int kernelSizeX, int kernelSizeY)
{
    int i, j, m, n;
    unsigned char *inPtr, *inPtr2, *outPtr;
    float *kPtr;
    int kCenterX, kCenterY;
    int rowMin, rowMax;                             // to check boundary of input array
    int colMin, colMax;                             //
    float sum;                                      // temp accumulation buffer

    // check validity of params
    if(!in || !out || !kernel) return false;
    if(dataSizeX <= 0 || kernelSizeX <= 0) return false;

    // find center position of kernel (half of kernel size)
    kCenterX = kernelSizeX >> 1;
    kCenterY = kernelSizeY >> 1;

    // init working  pointers
    inPtr = inPtr2 = &in[dataSizeX * kCenterY + kCenterX];  // note that  it is shifted (kCenterX, kCenterY),
    outPtr = out;
    kPtr = kernel;

    // start convolution
    for(i= 0; i < dataSizeY; ++i)                   // number of rows
    {
        // compute the range of convolution, the current row of kernel should be between these
        rowMax = i + kCenterY;
        rowMin = i - dataSizeY + kCenterY;

        for(j = 0; j < dataSizeX; ++j)              // number of columns
        {
            // compute the range of convolution, the current column of kernel should be between these
            colMax = j + kCenterX;
            colMin = j - dataSizeX + kCenterX;

            sum = 0;                                // set to 0 before accumulate

            // flip the kernel and traverse all the kernel values
            // multiply each kernel value with underlying input data
            for(m = 0; m < kernelSizeY; ++m)        // kernel rows
            {
                // check if the index is out of bound of input array
                if(m <= rowMax && m > rowMin)
                {
                    for(n = 0; n < kernelSizeX; ++n)
                    {
                        // check the boundary of array
                        if(n <= colMax && n > colMin)
                            sum += *(inPtr - n) * *kPtr;

                        ++kPtr;                     // next kernel
                    }
                }
                else
                    kPtr += kernelSizeX;            // out of bound, move to next row of kernel

                inPtr -= dataSizeX;                 // move input data 1 raw up
            }

            // convert negative number to positive
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);

            kPtr = kernel;                          // reset kernel to (0,0)
            inPtr = ++inPtr2;                       // next input
            ++outPtr;                               // next output
        }
    }

    return true;
}
