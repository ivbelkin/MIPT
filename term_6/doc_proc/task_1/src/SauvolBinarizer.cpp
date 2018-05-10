#include <iostream>
#include <cmath>

#include "SauvolBinarizer.h"
#include "Image.h"


CSauvolaBinarizer::CSauvolaBinarizer( int window_, double R_, double k_ ) :
    R( R_ ),
    k( k_ )
{
    if( window_ % 2 == 0 ) {
        ++window_;
    } 
    window = window_;
}

void CSauvolaBinarizer::Binarize( CImage& img )
{
    calcIntegralImage( img );
    calcIntegralSquares( img );
    int W = img.GetWidth();
    int H = img.GetHeight();
    for( int x = 0; x < W; ++x ) {
        for( int y = 0; y < H; ++y ) {
            int t = calkTreashold( x, y, W, H );
            if( img.GetXY( x, y ) < t ) {
                img.SetXY( x, y, 0 );
            } else {
                img.SetXY( x, y, 255 );
            }
        }
    }
}

double CSauvolaBinarizer::calkTreashold( int x, int y, int W, int H )
{
    double m = getMeanInWindow( x, y, W, H );
    double s = getStdInWindow( x, y, W, H );
    double t = m * ( 1.0 + k * ( s / R - 1.0 ) );
    return t;
}

void CSauvolaBinarizer::calcIntegralImage( CImage& img )
{
    copyImgToBuf( img, integralImage );
    if( R < 0 ) {
        R = getGlobalStd( integralImage );
    }
    calkCumSum( integralImage, img.GetWidth(), img.GetHeight() );
}

void CSauvolaBinarizer::calcIntegralSquares( CImage& img )
{
    copyImgToBuf( img, integralSquares );
    for( auto& v : integralSquares ) {
        v = v * v;
    }
    calkCumSum( integralSquares, img.GetWidth(), img.GetHeight() );
}

void CSauvolaBinarizer::copyImgToBuf( CImage& img, std::vector<int>& buf )
{
    int H = img.GetHeight();
    int W = img.GetWidth();
    buf.assign( W * H, 0 );
    for( int x = 0; x < W; ++x ) {
        for( int y = 0; y < H; ++y ) {
            buf[ix(x, y, W)] = img.GetXY( x, y );
        }
    }
}

void CSauvolaBinarizer::calkCumSum( std::vector<int>& a, int W, int H )
{
    for( int x = 1; x < W; ++x ) {
        a[ix(x, 0, W)] = a[ix(x - 1, 0, W)] + a[ix(x, 0, W)];
    }
    for( int y = 1; y < H; ++y ) {
        a[ix(0, y, W)] = a[ix(0, y - 1, W)] + a[ix(0, y, W)];
    }
    for( int x = 1; x < W; ++x ) {
        for( int y = 1; y < H; ++y ) {
            a[ix(x, y, W)] = a[ix(x - 1, y, W)] 
                             + a[ix(x, y - 1, W)] 
                             - a[ix(x - 1, y - 1, W)] 
                             + a[ix( x, y, W )];
        }
    }
}

double CSauvolaBinarizer::getSumInWindow( std::vector<int>& a, int x, int y, int W, int H )
{
    int up = getIntegral( a, x + window / 2, y - window / 2 - 1, W, H );
    int left = getIntegral( a, x - window / 2 - 1, y + window / 2, W, H );
    int diag = getIntegral( a, x - window / 2 - 1, y - window / 2 - 1, W, H );
    int current = getIntegral( a, x + window / 2, y + window / 2, W, H );
    return current - left - up + diag;
}

double CSauvolaBinarizer::getMeanInWindow( int x, int y, int W, int H )
{
    return getSumInWindow( integralImage, x, y, W, H ) / ( window * window );
}

double CSauvolaBinarizer::getStdInWindow( int x, int y, int W, int H )
{
    double mean = getMeanInWindow( x, y, W, H );
    double meanSquare = getSumInWindow( integralSquares, x, y, W, H ) / ( window * window );
    return sqrt(meanSquare - mean * mean);
}

double CSauvolaBinarizer::getGlobalStd( std::vector<int>& a )
{
    double mean = 0;
    double meanSquare = 0;
    for( int x : a ) {
        mean += x;
        meanSquare += x * x;
    }
    mean /= a.size();
    meanSquare /= a.size();
    return sqrt( meanSquare - mean * mean );
}

int CSauvolaBinarizer::getIntegral( std::vector<int>& a, int x, int y, int W, int H )
{
    if( 0 <= x && x < W && 0 <= y && y < H ) {
        return a[ix( x, y, W )];
    } else {
        return 0;
    }
}

inline int CSauvolaBinarizer::ix( int x, int y, int W ) const
{
    return x + y * W;
}
