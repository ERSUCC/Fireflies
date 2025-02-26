# install dependencies into fedora image

from fedora:latest as base

copy . .

run dnf install -y opencv-devel cmake g++ python

# build and run script on base image

from base

run mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build .

shell [ "/bin/bash", "-c" ]
cmd ./build/fireflies | python udp_client.py
