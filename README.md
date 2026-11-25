# MusicPlayer

## 1. 概述

原 VS2012 工程，已升级到 VS 18 (Visual Studio 2026)
使用Win32 API编程

详细设计见 [docs/设计文档.md](docs/设计文档.md)。

## 2. 源代码目录结构

```shell
MusicPlayer.sln
README.md
CMakeLists.txt
cmake/
  toolchain-zig-x86-windows.cmake
docs/
  设计文档.md
  设计文档.xlsx
  images/
    ui-layout.png
third_party/
  libzplay/
    libzplay.dll
    libzplay.h
    libzplay.lib
src/
  MusicPlayer.vcxproj
  MusicPlayer.vcxproj.filters
  app/
    MusicPlayer.cpp
    MusicPlayer.h
  audio/
    AudioPlayer.cpp
    AudioPlayer.h
  common/
    Common.cpp
    Common.h
    stdafx.cpp
    stdafx.h
    targetver.h
  ui/
    PlayerView.cpp
    PlayerView.h
  res/
    MusicPlayer.ico
    MusicPlayer.rc
    resource.h
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

程序启动后会把工作目录切到 exe 所在目录，并列出其中的 MP3/WAV 文件。

## 4. 使用 CMake 构建

Windows 下可以用 VS 18 打开仓库根目录，或者用 CMake 生成解决方案：

```shell
cmake -S . -B build-vs -G "Visual Studio 18 2026" -A Win32
cmake --build build-vs --config Debug
```

生成的解决方案是 build-vs/MusicPlayer.sln，产物在 build-vs/Debug/。
仓库根目录的 MusicPlayer.sln 是手工维护的 VS 工程，两者都可以编译。

Linux 下可以使用 CMake + zig cc 交叉编译 32 位 Windows 程序，
需要安装 cmake、zig 和 iconv：

```shell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-zig-x86-windows.cmake
cmake --build build
```

编译产物是 build/MusicPlayer.exe，libzplay.dll 会自动拷贝到同一目录。
MusicPlayer.rc 是 UTF-16LE 编码，MinGW/zig 构建时会先转成 UTF-8 再交给 zig rc。
zig 构建使用自带的 MinGW/UCRT，建议在 Windows 10 及以上运行。
