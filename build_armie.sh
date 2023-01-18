cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=armclang \
    -DCMAKE_CXX_COMPILER=armclang \
    -DBUILD_WITH_SVE2=ON \
    ..