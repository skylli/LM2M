# 1. creat m2m library.
# cmake version request
cmake_minimum_required(VERSION 2.6)
# define project name
project (lm2m C)
# The version number.
set(M2M_VERSION_MAJOR 0)
set(M2M_VERSION_MINOR 0)
set(M2M_VERSION_PATCH 1) 

# 路劲设置
SET( SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET( PLATFORM_PATH_LINUX ${PROJECT_SOURCE_DIR}/platform/linux)
SET( APP_PATH ${PROJECT_SOURCE_DIR}/app)
SET( TST_PATH ${PROJECT_SOURCE_DIR}/test)

#目标文件放置到 bin 文件夹
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
SET( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置库文件名
set( m2m_lib "lm2m_lib.${M2M_VERSION_MAJOR}.${M2M_VERSION_MINOR}.${M2M_VERSION_PATCH}")
set( m2m_share "lm2m.${M2M_VERSION_MAJOR}.${M2M_VERSION_MINOR}.${M2M_VERSION_PATCH}")

#configure_file(
#    "${PROJECT_SOURCE_DIR}/config/cmakeValue.h.in"
#   "${PROJECT_SOURCE_DIR}/include/BuildValue.h"
#)

# 搜索当前目录下所有的头文件
# file(GLOB_RECURSE Foo_HEADERS "*.h")
# set (Foo_INCLUDE_DIRS "")
# foreach (_headerFile ${Foo_HEADERS})
#     get_filename_component(_dir ${_headerFile} PATH)
#     list (APPEND Foo_INCLUDE_DIRS ${_dir})
# endforeach()
# list(REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

# 包含工程所有的头文件
# include_directories(${Foo_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 子模块生成子库
FILE(GLOB SUBLIB_SRC ${SRC_PATH}/*.c)
FILE(GLOB SUBLIB_PLATFORM ${PLATFORM_PATH_LINUX}/*.c)
# 编译器配置
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 生成共享库
ADD_LIBRARY(lm2m STATIC ${SUBLIB_SRC} ${SUBLIB_PLATFORM})
SET_TARGET_PROPERTIES(lm2m PROPERTIES LINKER_LANGUAGE C)
#ADD_LIBRARY(${m2m_share} SHARED ${SUBLIB_UTIL} ${SUBLIB_PLATFORM} ${SUBLIB_CRYPT} ${SUBLIB_NETWORK} ${SUBLIB_API} ${SUBLIB_THREE_HADC})

# sample 应用实例
# If necessary, use the RELATIVE flag, otherwise each source file may be listed 
# with full pathname. RELATIVE may makes it easier to extract an executable name

# sample 应用实例
FILE( GLOB APP_SOURCES  ${APP_PATH}/*.c )
foreach( applicationfile ${APP_SOURCES} )
    # I used a simple string replace, to cut off .cpp.
    #string( REPLACE ".c" "" testname ${testsourcefile} )
    get_filename_component(APP_EXE ${applicationfile} NAME_WE)
    add_executable( ${APP_EXE} ${applicationfile} )
    # Make sure YourLib is linked to each app
    target_link_libraries( ${APP_EXE} lm2m)
endforeach( applicationfile ${APP_SOURCES} )
