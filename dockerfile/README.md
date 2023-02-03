## Docker 镜像生成

### 1 Dockerfile.arm64.v8.qemu.sve2
> 基于 Arm Instruction Emulator 运行测试，相关参考 [Arm Instruction Emulator Developer and Reference Guide](https://developer.arm.com/documentation/102190/22-0)
> 该镜像可用于 M1

###### 1.1 依赖
- arm64v8/ubuntu:18.04
- arm-compiler（需下载）: https://developer.arm.com/downloads/-/arm-compiler-for-linux
- ARM-Instruction-Emulator（需下载）: https://developer.arm.com/downloads/-/arm-instruction-emulator

###### 1.2 镜像生成
`docker build -t ${IMAGE_NAME} -f Dockerfile.arm64.v8.qemu.sve2 .`

### 2 Dockerfile.x86.qemu.sve2
> 基于 qemu 运行测试

###### 2.1 镜像生成
`docker build -t ${IMAGE_NAME} -f Dockerfile.x86.qemu.sve2 .`
