# simple-software-renderer
This is a simple software renderer used as an educational exercise for both C++ and software rasterization.

## compilation

To compile the simple software renderer, you need to add GLM, and SDL2. As a compilation tool CMAKE is used.

In Debian based distros the necessary commands for the first compilation are:
```
sudo apt install libsdl2-dev cmake
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
./ssr
```
