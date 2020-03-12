
mkdir -p build/p
cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j1 && ./engine ../game0
cd ..