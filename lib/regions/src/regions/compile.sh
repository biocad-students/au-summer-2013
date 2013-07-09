#!/bin/bash

cd ./build
cmake ../src/ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-std=c++11
make
make install
