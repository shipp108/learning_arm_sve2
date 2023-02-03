cmake \
    -DCMAKE_TOOLCHAIN_FILE=${NDK25_HOME}/build/cmake/android.toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-24 \
    -DANDROID_STL=c++_static \
    -DANDROID_TOOLCHAIN=clang \
    -DBUILD_WITH_SVE2=ON \
    ..