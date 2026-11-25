# MusicPlayer

## 1. 概述

VS2012 创建的工程
使用WINAPI编程

## 2. 源代码目录结构

```shell
MusicPlayer.sln
README.md
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
