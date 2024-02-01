<div align=center>
  <img width=64 src="app.ico">
</div>

<h1 align="center">
  SAST Music
</h1>
<p align="center">
  NJUPT SAST2023.C++ WoC
</p>

<p align="center">
<img src="https://img.shields.io/badge/frame-Qt6.6.1-blue">
<img src="https://img.shields.io/badge/std-C++17-yellow">
<img src="https://img.shields.io/badge/platform-Windows-white">
<img src="https://img.shields.io/badge/platform-Linux-green">
<img src="https://img.shields.io/badge/platform-macOS-orange">
</p>

## 简介

桌面端云音乐播放器，使用Qt6编写

## Build

fork 本仓库后，执行以下命令

> [!NOTE]
> 请将 `<YOUR_USERNAME>/<YOUR_REPO_NAME>` 更换为你的用户名和仓库地址

```bash
git clone --recursive https://github.com/<YOUR_USERNAME>/<YOUR_REPO_NAME>.git 
```

Use your IDE (QtCreator or CLion) to open. Only `CMake` support.

### 已知问题及解决方法

- **Linux平台CMake生成报错**
    
    保证目录下存在build文件夹后再执行CMake，或执行第二遍CMake

- **Linux平台编译报错，无法创建文件夹 `/usr/lib/qt6/qml/FluentUI`**
  
    ```shell
    sudo chmod 777 /usr/lib/qt6/qml
    ```

- **Linux平台运行报错：`use incomplete Qt library(5.15)`**

    将 `libs/FluentUI/CMakeLists.txt` 中

    ```cmake
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    ```
   
    改为
   
    ```cmake
    find_package(QT NAMES Qt6REQUIRED COMPONENTS Core)
    ```

    重新编译后再将其改回去

- **CMake `find_package` 报错**
    
    下载安装缺少的组件，例如 `Qt6::MultiMedia`

## Tasks & API Docs

[wiki](https://github.com/NJUPT-SAST-Cpp/sast-music/wiki/API-Docs)
