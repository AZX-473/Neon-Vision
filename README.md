# NeonVision

## 简述/介绍
通过 ImGui , OpenGL , GLFW 实现的简单图像处理软件

## 目的/功能
通过图形化界面和各种绘制功能来达到辅助电脑使用的目的

## 运行环境
Windows 10/11

## 编译
请确保已安装 CMake , GCC 或其余编译环境


1.安装 vcpkg
``` BASH
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
```
2.配置vcpkg环境变量


3.安装依赖库
``` BASH
vcpkg install imgui:x64-windows
vcpkg install glfw3:x64-windows
vcpkg install opengl:x64-windows
vcpkg install imgui[core,glfw-binding,opengl3-binding]:x64-windows
```
4.编译项目
在项目根目录下执行
``` BASH
cmake -B build -A x64
cmake --build build --config Release
```
5.构建完成的可执行文件位于 build/Release 目录下
```BASH
.\build\Release\NeonVision.exe
```

## 安装
### [请跳转至realease页面下载最新的安装包](https://github.com/AZX-473/Neon-Vision/release)