cmake_minimum_required(VERSION 2.8)

project(OpenCLPlayAround)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package( OpenCV REQUIRED )
find_package(PCL 1.8 REQUIRED )
find_package(OpenCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})

add_definitions(${PCL_DEFINITIONS})
link_directories(${PCL_LIBRARY_DIRS})
link_directories(${OpenCL_LIBRARY})

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core ${OpenCV_LIBS} ${PCL_LIBRARIES} ${OpenCL_LIBRARY})
