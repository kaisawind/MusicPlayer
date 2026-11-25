# MusicPlayer

## 1. 概述

VS2012 创建的工程
使用WINAPI编程

## 2. 源代码目录结构

```shell
MusicPlayer.sln
README.md
CMakeLists.txt
cmake/
  toolchain-zig-x86-windows.cmake
docs/
  设计文档.xlsx
third_party/
  libzplay/
    libzplay.dll
    libzplay.h
    libzplay.lib
src/
  CFilesProcessor.cpp
  CFilesProcessor.h
  CMusicProcessor.cpp
  CMusicProcessor.h
  Common.cpp
  Common.h
  CViewerProcessor.cpp
  CViewerProcessor.h
  MusicPlayer.cpp
  MusicPlayer.h
  MusicPlayer.vcxproj
  MusicPlayer.vcxproj.filters
  ReadMe.txt
  resource.h
  stdafx.cpp
  stdafx.h
  targetver.h
  res/
    MusicPlayer.ico
    MusicPlayer.rc
    small.ico
```

## 3. 编译及运行

编译工程时会在仓库根目录产生Debug或者Release文件夹
需要将third_party\libzplay\libzplay.dll拷贝到
Debug或者Release文件夹下

目录结构
```shell
MusicPlayer
  Debug
    libzplay.dll
    ...
    MusicPlayer.exe
```

## 4. 使用 CMake + zig cc 交叉编译

Linux 下可以使用 CMake + zig cc 交叉编译 32 位 Windows 程序，
需要安装 cmake、zig 和 iconv。

```shell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-zig-x86-windows.cmake
cmake --build build
```

编译产物是 build/MusicPlayer.exe，libzplay.dll 会自动拷贝到同一目录。
MusicPlayer.rc 是 UTF-16LE 编码，构建时会先转成 UTF-8 再交给 zig rc。

注意：Windows 原生构建仍使用 MusicPlayer.sln；CMake 构建使用 zig 自带的
MinGW/UCRT，建议在 Windows 10 及以上运行。
