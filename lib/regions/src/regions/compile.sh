#!/bin/bash

cd ./build
cmake ../src/ -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
make
make install