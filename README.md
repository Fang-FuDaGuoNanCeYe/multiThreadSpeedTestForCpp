# multiThreadSpeedTestForCpp
version 0.0.1 (20260212)

# Introduction 简介

This project highlights the efficiency difference between various multi-thread structures. It can compare 4 widely-used CPU multi-thread structures with intel (one)TBB, and with CUDA.

这是一个用于测试C++中不同多线程结构效率差异的项目。目前可以对比4种常见的CPU多线程实现方法、intel (one)TBB和CUDA。

</br>

Four widely-used structures are:

四个常见的CPU多线程实现方法分别是：

1. mutex lock
2. atomic process ctrl (`sub_fetch(1)` one by one)
3. atomic process ctrl (`sub_fetch(size * 0.7/threadNum)` for the first time, then `fetch_sub(1)`)
4. work stealing


# Compile Environment Requirements 编译环境要求

To compile `main.cpp` which is able to compare four widely-used CPU multi-thread structure with intel TBB, a compiler supporting cpp23 (i.e. supporting `std::print("{}",std::duration)`) is needed. 

编译`main.cpp`需要支持cpp23的编译器（至少支持`std::print("{}",std::duration)`）。

</br>

To compile `mainCuda.cu` which contains the CUDA, it can be compiled by cudaToolkit 12.8 and cpp17.

`mainCuda.cu`的编译可以通过cudaToolkit 12.8 + cpp17来实现。

# How To Use 使用方法

1. Download the entire respiratory.
2. For windows users, in the `compile` folder are two `.ps1` files, which is the compile&run code I used. It is suggested that the content of these files should be adjusted for your computer before running.
3. Run the two `.ps1` code one by one.

Multi(Cross)-platform support may be done in the future.

</br>

1. 下载整个仓库的所有内容
2. 对于windows用户，建议您先看下`compile`文件夹里面的两个`.ps1`文件，把它们改成适合您的电脑的形式，然后运行这两个`.ps1`文件即可编译运行。它们是我使用的编译命令。

我也许会在将来的某天做跨平台支持。
