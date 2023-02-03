# learning SVE2

## 1 模拟器镜像构建
[docker build](dockerfile/README.md)

## 2 armie 模拟器

#### 2.1 编译
参考 [build_armie](build_script/build_armie.sh) 脚本

#### 2.2 执行
```
# 模拟执行
armie -msve-vector-bits=128 ./app_bin

# 模拟指令统计, 具体用法移步 https://developer.arm.com/documentation/102190/22-0?lang=en
armie -msve-vector-bits=512 -i libinscount_emulated.so ./app_bin
armie -msve-vector-bits=512 -i libopcodes_emulated.so ./app_bin
```

## 3 qemu 模拟器

#### 2.1 编译
参考 [build_qemu](build_script/build_qemu.sh) 脚本

#### 2.2 执行
```
# 模拟执行
armie -msve-vector-bits=128 ./app_bin

# 模拟指令统计, 具体用法移步 https://developer.arm.com/documentation/102190/22-0?lang=en
armie -msve-vector-bits=512 -i libinscount_emulated.so ./app_bin
armie -msve-vector-bits=512 -i libopcodes_emulated.so ./app_bin
```


## 4 android binary
> 需要安装支持 SVE 编译的 NDK 版本（NDK 24 及以上版本）

#### 4.1 编译
参考 [build_android_ndk](build_script/build_qemu.sh) 脚本

#### 4.2 执行
binary 测试文件，可使用 adb shell 执行，非 root android 机，可 push 至 /data/local/tmp 目录进行测试