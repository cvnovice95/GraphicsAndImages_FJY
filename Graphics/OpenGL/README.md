# 文件结构:

### README.md
### |-----DLL/       
### ||-----glu32.dll
### ||-----glut32.dll
### ||-----glaux.dll
### |-----LIB/
### ||-----glu32.lib
### ||-----glut32.lib
### ||-----opengl32.lib
### |-----INCLUDE/
### ||-----gl.h
### ||-----glu.h
### ||-----glut.h
### ||-----glaux.h
### |-----BIN/
### |-----OPENGLWORK/
### ||-----CGL.bat
### ||-----TEST.c



# 文件描述:

## 1.Windows下 VS2013配置OPENGL

### 将DLL文件下的DLL文件放入 系统目录:\Windows\System32   系统目录:\Windows\SysWOW64
### 将LIB文件下的lib文件放入 VS2013安装目录:\VC\lib
### 将INCLUDE文件下的.h文件放入 VS2013安装目录:\VC\lib\GL
## 2.Windows下 gcc编译环境下使用OPENGL

### 设置系统环境变量 在环境变量PATH中 添加路径指向BIN\bin
### 在OPENGLWORK下存放c程序文件
### 打开cmd 进入OPENGLWORK 输入CGL c程序文件名.c 自定义可执行文件名 在当前目录下产生可执行文件。



文件说明:
1. \Algorithm\CircleAlgorithm.c 计算机图形学画圆算法
2. \Algorithm\LineAlgorithm.c   计算机图形学画直线算法
3. \SerialPort_OpenGL\MAIN.cpp   重磁力传感器通过串口传输数据通过OpenGL显示姿态 