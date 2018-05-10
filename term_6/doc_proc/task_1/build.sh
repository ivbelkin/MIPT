#! /bin/bash

g++ src/main.cpp src/SauvolBinarizer.cpp -o binarizer -std=c++11 -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
