# A C++ Implementation of Konno and Ohmachi Smoothing

> Joseph P. Vantassel, The University of Texas at Austin

## Getting Started

### Parallel

1. Download or clone the repository.
2. Run: `mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_BUILD_OMP=yes ..`.
3. Run: `make clean && make`.
4. Run: `./smooth ../examples/data.csv smooth_data.csv`.

### Serial

Same as parallel, except with the second step swapped with:

2. Run: `mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_OMP=no ..`.
