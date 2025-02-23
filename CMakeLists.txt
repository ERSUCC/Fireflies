cmake_minimum_required(VERSION 3.21)

project(fireflies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(fireflies list.cpp
                         main.cpp)

target_include_directories(fireflies PRIVATE opencv/build/include)

if (WIN32)
    target_link_directories(fireflies PRIVATE opencv/build/x64/vc16/lib)
else()
    target_link_directories(fireflies PRIVATE opencv/build/lib)
endif()

target_link_libraries(fireflies opencv_world4110d)
