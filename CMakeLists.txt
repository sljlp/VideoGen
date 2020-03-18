cmake_minimum_required(VERSION 3.0)
project (VideoGen) #project_name是你要创建的工程名
find_package(OpenCV 4.1 REQUIRED)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/json JSON_SRCS)
add_executable(VideoGen ${DIR_SRCS} ${JSON_SRCS})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(VideoGen ${OpenCV_LIBS})

FIND_PACKAGE( OpenMP)


# to be changed
if(OPENMP_FOUND)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${OpenMP_CXX_FLAGS}")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(OPENMP_FOUND)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${OpenMP_CXX_FLAGS}")
