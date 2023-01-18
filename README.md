## learning SVE2

## armie 执行指令
```
# 模拟执行
armie -msve-vector-bits=512 ./app_bin

# 模拟指令统计, 具体用法移步 https://developer.arm.com/documentation/102190/22-0?lang=en
armie -msve-vector-bits=512 -i libinscount_emulated.so ./app_bin
armie -msve-vector-bits=512 -i libopcodes_emulated.so ./app_bin
```