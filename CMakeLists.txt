cmake_minimum_required(VERSION 3.21)

project(fireflies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(fireflies src/list.cpp
                         src/main.cpp)

target_include_directories(fireflies include)

if (WIN32)
    target_include_directories(fireflies PRIVATE opencv/build/include)

    target_link_directories(fireflies PRIVATE opencv/build/x64/vc16/lib)
    target_link_libraries(fireflies opencv_world4110d)
elseif (APPLE)
    target_include_directories(fireflies PRIVATE /usr/local/include/opencv4)

    target_link_directories(fireflies PRIVATE /usr/local/lib)
    target_link_libraries(fireflies opencv_core opencv_highgui opencv_imgproc opencv_videoio)
else ()
    target_include_directories(fireflies PRIVATE /usr/include/opencv4)

    target_link_directories(fireflies PRIVATE /usr/lib)
    target_link_libraries(fireflies opencv_core opencv_highgui opencv_imgproc opencv_videoio)
endif()
