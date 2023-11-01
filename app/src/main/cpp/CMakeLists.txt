# 声明CMake最低版本。
cmake_minimum_required(VERSION 3.18.1)

# 设置编译器。
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall") # 设置C编译器。
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall") # 设置C++编译器。
set(OPENGL_LIB GLESv3) # 设置OPENGL库。

# 声明项目名称
project(Native)

# 声明一个库，设置为动态库或者静态库，并且提供源码的相对路径（可以由多个源码路径文件来生成一个库）。
# Gradle通过这个配置自动打包并分享库到你的APK中。
add_library(
        Native  # 设置库名称。
        SHARED # 设置库为动态库或者静态库。
        native/Native.cpp # 提供源码的相对路径。
)
add_library(Utils SHARED native/util/LoadUtil.cpp native/util/CameraUtil.cpp native/include/LogUtil.h)
add_library(Triangle SHARED native/lesson1/Triangle.cpp)
add_library(Cube SHARED native/lesson2/Cube.cpp)
add_library(TextureCube SHARED native/lesson3/TextureCube.cpp)

# 搜索指定预定义库并存储它们的路径作为变量，因为CMake默认包含系统库在搜索路径中，所以你只需要指定要加入的NDK库的名称。
# CMake在完成构建之前验证库是否存在。
find_library(
        log-lib # 设置路径变量名称。
        log # 指定你想让CMake定位的NDK库的名称。
)

# 指定CMake链接到你的目标库的库，你可以链接多个库，例如你在这个构建脚本中定义的库，预构建的第三方库或者系统库。
target_link_libraries(
        Native # 指定目标库。
        Cube # 链接目标库到cube库
        Utils # 链接目标库到utils库。
)
target_link_libraries(
        Utils
        ${OPENGL_LIB} # 链接OPENGL库。
        EGL # 链接EGL库。
        ${log-lib} # 链接目标库到NDK中包含的日志库。
)
target_link_libraries(Triangle Utils ${OPENGL_LIB} EGL)
target_link_libraries(Cube Utils ${OPENGL_LIB} EGL)
target_link_libraries(TextureCube Utils ${OPENGL_LIB} EGL)