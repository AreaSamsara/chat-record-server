# 指定构建此项目所需的最低CMake版本
cmake_minimum_required (VERSION 3.14)

project (demo)

# 设置构建类型，如果没有指定则默认为Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# 设置C++标准，并强制要求编译器必须支持此标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# 查找 Boost 库
find_package(Boost 1.86 REQUIRED COMPONENTS system filesystem)
# 查找MySQL客户端库
find_package(PkgConfig REQUIRED)
pkg_check_modules(MYSQL REQUIRED mysqlclient)

# 引入FetchContent模块
include(FetchContent)

FetchContent_Declare(
    jsoncpp
    # GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_REPOSITORY https://gitee.com/chooosky/jsoncpp
    GIT_TAG        1.9.5
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://gitee.com/mirrors/fmt.git
    GIT_TAG        10.2.1
)
FetchContent_Declare(
  cpp-httplib
  # GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_REPOSITORY https://gitee.com/YeLee_CN/cpp-httplib.git
  GIT_TAG        v0.14.0
)
FetchContent_Declare(
    spdlog
    # GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_REPOSITORY https://gitee.com/mirror-luyi/spdlog.git
    GIT_TAG        v1.14.1
)
FetchContent_Declare(
  soci
  # GIT_REPOSITORY https://github.com/SOCI/soci.git
  GIT_REPOSITORY https://gitee.com/mirrors_SOCI/soci
  GIT_TAG master
  CMAKE_ARGS -DWITH_MYSQL=ON
)

FetchContent_MakeAvailable(
    jsoncpp 
    fmt 
    cpp-httplib
    spdlog
    soci
)

# 先创建目标，再动态添加源文件
add_executable(main src/main.cpp)
target_sources(main PRIVATE 
    src/server/server.cpp
    src/server/api.cpp
    src/log/log.cpp
    src/database/database.cpp
)

# 指定编译器在搜索头文件时，应包含项目中的include目录
target_include_directories(main PRIVATE include)

# 添加MySQL头文件路径
target_include_directories(main PRIVATE 
    ${MYSQL_INCLUDE_DIRS}
)

# 将可执行文件main与库进行链接，PRIVATE表示该库仅用于构建main本身
target_link_libraries(main PRIVATE 
    Boost::system
    Boost::filesystem
    ${MYSQL_LIBRARIES}

    jsoncpp_lib
    fmt::fmt
    httplib::httplib
    spdlog::spdlog
    soci_core
    soci_mysql
)