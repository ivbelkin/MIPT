#pragma once

#include <vector>

#include "Image.h"

class CSauvolaBinarizer {
public:
    CSauvolaBinarizer( int window_, double R_, double k_ );
    void Binarize( CImage& img );

private:
    // параметры алгоритма
    int window;
    double R;
    double k;

    std::vector<int> integralImage;
    std::vector<int> integralSquares;
    void calcIntegralImage( CImage& img );
    void calcIntegralSquares( CImage& img );
    void calkCumSum( std::vector<int>& a, int W, int H );
    void copyImgToBuf( CImage& img, std::vector<int>& buf );
    double getSumInWindow( std::vector<int>& a, int x, int y, int W, int H );
    double getMeanInWindow( int x, int y, int W, int H );
    double getStdInWindow( int x, int y, int W, int H );
    double getGlobalStd( std::vector<int>& a );
    int getIntegral( std::vector<int>& a, int x, int y, int W, int H );
    double calkTreashold( int x, int y, int W, int H );
    inline int ix( int x, int y, int W ) const;
};
