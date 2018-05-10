#pragma once

#include <string>
#include <algorithm>
#include "CImg.h"

class CImage {

public:
    explicit CImage( const char* filename_ );

    inline void SetXY( int x, int y, int value );
    inline void SetXYC( int x, int y, int c, int value );
    inline unsigned char GetXY( int x, int y ) const;
    inline unsigned char GetXYC( int x, int y, int c ) const;
    inline int GetHeight() const;
    inline int GetWidth() const;
    inline void Display() const;
    inline void SavePng( const char* filename ) const;

private:
    cimg_library::CImg<unsigned char> img;
    std::string filename;
};

inline CImage::CImage( const char* filename_ ) :
    img( filename_ ),
    filename( filename_ )
{}

inline void CImage::SetXY( int x, int y, int value )
{
    SetXYC( x, y, 0, value );
    SetXYC( x, y, 1, value );
    SetXYC( x, y, 2, value );
}

inline void CImage::SetXYC( int x, int y, int c, int value )
{
    *img.data( x, y, 0, c ) = value;
}

inline unsigned char CImage::GetXY( int x, int y ) const
{
    return 0.3 * GetXYC( x, y, 0 ) + 0.59 * GetXYC( x, y, 1 ) + 0.11 * GetXYC( x, y, 2 );
}

inline unsigned char CImage::GetXYC( int x, int y, int c ) const
{
    return *img.data( x, y, 0, c );
}

inline int CImage::GetHeight() const
{
    return img.height();
}

inline int CImage::GetWidth() const
{
    return img.width();
}

inline void CImage::Display() const
{
    img.display( filename.c_str() );
}

inline void CImage::SavePng( const char* filename ) const
{
    img.save_png( filename, 1 );
}
