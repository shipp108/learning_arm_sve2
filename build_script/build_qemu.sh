cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc-10 \
    -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++-10 \
    -DCMAKE_EXE_LINKER_FLAGS="-static" \
    -DBUILD_WITH_SVE2=ON \
    ..