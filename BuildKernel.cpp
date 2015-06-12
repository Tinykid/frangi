//
//  BuildKernel.cpp

#include "BuildKernel.h"

using namespace Eigen;

float Pi = 3.14159265359;
float Sigma = 2.5;

bool BuildKernel ()
{

// Make kernel coordinates
int Grid_X = round(3*Sigma);
int Grid_nX = -round(3*Sigma);
int Grid_Y = round(3*Sigma);
int Grid_nY = -round(3*Sigma);
int XSize_Matrix = Grid_X + abs(Grid_nX) + 1;
int YSize_Matrix = Grid_Y + abs(Grid_nY) + 1;

MatrixXi KernelX(YSize_Matrix,XSize_Matrix);
MatrixXi KernelY(YSize_Matrix,XSize_Matrix);

for (int iiy = 0 ; iiy != YSize_Matrix ; ++iiy) //Loop over rows
{
    for (int iix = 0 ; iix != XSize_Matrix ; ++iix) //Loop over columns
    {
        KernelX(iiy,iix) = Grid_nX + iiy;
        KernelY(iix,iiy) = Grid_nY + iiy;
    }
}

///////////////////////////////////////////////////////////////////////////////
/*
 //Error check
 //Output kernels
 printf("\nSigma: %f ; Grid_X: %d ; Grid_Y: %d\n", Sigma, Grid_X, Grid_Y);
 std::ofstream kx;
 std::ofstream ky;
 kx.open ("kx.txt");
 kx << KernelX;
 kx.close();
 ky.open ("ky.txt");
 ky << KernelY;
 ky.close();
*/
///////////////////////////////////////////////////////////////////////////////

MatrixXf DGaussxx(YSize_Matrix,XSize_Matrix);
MatrixXf DGaussxy(YSize_Matrix,XSize_Matrix);
MatrixXf DGaussyy(YSize_Matrix,XSize_Matrix);

// Build the gaussian 2nd derivatives kernels
for(int iy = 0 ; iy != YSize_Matrix ; ++iy)//Loop over rows
{
    for(int ix = 0 ; ix != XSize_Matrix ; ++ix)//Loop over columns
    {
        DGaussxx(iy,ix) = 1/(2*Pi*pow(Sigma,4)) * (pow(KernelX(iy,ix),2)/pow(Sigma,2)-1) * exp(-(pow(KernelX(iy,ix),2)+pow(KernelY(iy,ix),2))/(2*pow(Sigma,2)));
        DGaussxy(iy,ix) = 1/(2*Pi*pow(Sigma,6)) * (KernelX(iy,ix) * KernelY(iy,ix)) * exp(-(pow(KernelX(iy,ix),2)+pow(KernelY(iy,ix),2))/(2*pow(Sigma,2)));
    }
}

DGaussyy = DGaussxx.transpose();

///////////////////////////////////////////////////////////////////////////////
/*
 //Error check
 //Output gaussian kernels
 printf("\nSigma: %f ; Grid_X: %d ; Grid_Y: %d\n", Sigma, Grid_X, Grid_Y);
 std::ofstream gaussxx;
 std::ofstream gaussxy;
 std::ofstream gaussyy;
 gaussxx.open ("gaussxx.txt");
 gaussxx << DGaussxx;
 gaussxx.close();
 gaussxy.open ("gaussxy.txt");
 gaussxy << DGaussxy;
 gaussxy.close();
 gaussyy.open ("gaussyy.txt");
 gaussyy << DGaussyy;
 gaussyy.close();
*/
///////////////////////////////////////////////////////////////////////////////

//Rewrite the gaussian kernels as a linear float
int KLength = XSize_Matrix * YSize_Matrix;
    float DGaussxxL[KLength];
    float DGaussxyL[KLength];
    float DGaussyyL[KLength];

int iiiL = 0;
for (int iiiy = 0 ; iiiy != YSize_Matrix ; ++iiiy)
{
    for (int iiix = 0 ; iiix != XSize_Matrix ; ++iiix, ++iiiL)
    {
        DGaussxxL[iiiL] = DGaussxx(iiiy,iiix);
        DGaussxyL[iiiL] = DGaussxy(iiiy,iiix);
        DGaussyyL[iiiL] = DGaussyy(iiiy,iiix);
    }
}

///////////////////////////////////////////////////////////////////////////////
/*
 //Error check
 //Output linearized kernels
 std::ofstream gaussxxl;
 std::ofstream gaussxyl;
 std::ofstream gaussyyl;
 gaussxxl.open ("gaussxxl.txt");
 for (int ik = 0 ; ik != KLength ; ++ik)
 {
 gaussxxl << DGaussxxL[ik] << std::endl;
 
 };
 gaussxxl.close();
 gaussxyl.open ("gaussxyl.txt");
 for (int ik = 0 ; ik != KLength ; ++ik)
 {
 gaussxyl << DGaussxyL[ik] << std::endl;
 };
 gaussxyl.close();
 gaussyyl.open ("gaussyyl.txt");
 for (int ik = 0 ; ik != KLength ; ++ik)
 {
 gaussyyl << DGaussyyL[ik] << std::endl;
 };
 gaussyyl.close();
*/
///////////////////////////////////////////////////////////////////////////////
    
    return true;
}
