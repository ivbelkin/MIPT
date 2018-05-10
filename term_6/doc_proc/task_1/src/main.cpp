#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Image.h"
#include "SauvolBinarizer.h"

using namespace std;


int main( int argc, char** argv )
{
    const int w = 17;
    const double R = -1;
    const double k = 0.2;

    if( argc != 2 ) {
        cout << "Usage: binarizer <file with image filenames>\n";
        return 0;
    }

    ifstream fin( argv[1] );
    string filename;
    while( fin >> filename ) {
        string outname = filename + ".png";
        CImage img( filename.c_str() );
        CSauvolaBinarizer binarizer( w, R, k );
        timespec start;
        timespec finish;
        cout << "Processing " << filename;

        clock_gettime( CLOCK_MONOTONIC, &start );
        binarizer.Binarize( img );
        clock_gettime( CLOCK_MONOTONIC, &finish );

        cout << " Speed: ";
        double speed = static_cast<double>(
            ( finish.tv_sec * 1000000000 + finish.tv_nsec ) - ( start.tv_sec * 1000000000 + start.tv_nsec ))
            / ( img.GetHeight() * img.GetWidth() );
        cout << speed / 1000;
        cout << " sec/MP.";
        cout << " Saving to " << outname << endl;
        img.SavePng( outname.c_str() );
    }
    return 0;
}
