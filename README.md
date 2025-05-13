# Fireflies

## Overview

The goal of this project is to develop a low-latency, closed-loop system that enables researchers at the Northwestern Institute on Complex Systems to conduct real-time experiments on firefly synchronization. Motivated by observations of synchronized flashing in fireflies from Thailand, the system is designed to investigate the emergence of collective behavior by measuring and modulating the system’s order parameter.

The system integrates a high-speed camera to track individual firefly flashes and a real-time processing pipeline that analyzes this data and triggers LED responses with minimal delay. To achieve the required responsiveness, all core components responsible for flash detection and data processing are implemented in C++. This design decision ensures sub-10 millisecond latency, which is critical for responsively reacting to the natural flashing rhythms of the fireflies.

To make experimentation accessible to researchers with limited programming experience, the experimental logic is exposed via a Python interface. Researchers can define and modify experiment protocols in Python without interacting directly with the C++ core, allowing for rapid iteration and ease of use while maintaining high system performance.

## Demo

_Insert demo here once it's made_

## Hardware Requirements

- Computer with any modern operating system
- Raspberry Pi (find specs)
- Ethernet cable to connect computer to Raspberry Pi
- LEDs (find specs)
- Power source for Raspberry Pi (USB-C)
- A camera capable of transmitting real-time video data over USB

## System Requirements

The detection script is written in C++, and the experimental scripts are written in Python. The tools can be run on any platform, provided that the following dependencies are installed.

### CMake

CMake is used for coordinating cross-platform compilation of the C++ source code. Downloads for each operating system can be found at the following link: https://cmake.org/download/. Version 3.31 should be used for most likely compatibility with the provided source code and build files.

On Windows, you will also need MSVC, which can be found at the following link: https://visualstudio.microsoft.com/vs/features/cplusplus/. Unfortunately there is not currently a way to install the compiler without installing the Visual Studio application.

On Mac and Linux, you will also need G++, which should be pre-installed.

### Python

The Python runtime environment is used to run the analysis scripts. Downloads for each operating system can be found at the following link: https://www.python.org/downloads/. Version 3.12 should be used for most likely compatibility with the provided source code.

### OpenCV

OpenCV is used to read data from the camera and display a live preview of the camera feed. Downloads for each operating system can be found at the following link: https://opencv.org/releases/. Version 4.11 should be used for most likely compatibility with the provided source code.

## Raspberry Pi Requirements

Version of Raspberry Pi OS
Version of Python libraries
Make sure to install all of these dependencies

## Directory Structure

The published version of the project is as follows:

```bash
Fireflies
├── experiments
│   └── mirror_experiment.py
├── include
│   └── list.h
├── raspberry_pi
│   └── led_controller.py
├── src
│   ├── list.cpp
│   └── main.cpp
├── videos/
├── .gitignore
├── CMakeLists.txt
└── README.md
```

The `raspberry_pi` directory contains code that runs on the Raspberry Pi, as opposed to the computer. The `experiments` directory is where researchers can add their own experiments. The `src` and `include` directories contain the source code for the detection and analysis of firefly flashes that runs on the computer.

## Connecting to Pi

The above steps will enable the detection and analysis of the firefly movements, but in order to perform experiments with LEDs, the computer running these programs must also be connected to a Raspberry Pi.

At a high level, the computer is sending UDP packets containing indices representing the specific LEDs that should be turned on. On the Raspberry Pi side, it will receive these packets and turn on the LEDs and then turn off the LEDs after a certain period of time (which can be configured). Therefore, all the computer is responsible for is telling the Raspberry Pi which LEDs to turn on and when.

To accomplish this, connect the Raspberry Pi to the computer with an Ethernet cable. Statically configure the IP address of the Raspberry Pi and the laptop to 10.0.0.42 and 10.0.0.41 respectively.

## Building

The C++ code can be compiled with the following commands, beginning in the directory that contains CMakeLists.txt.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

On Windows, these commands will place the final executable in the directory `build\Debug`. On Mac and Linux, these commands will place the final executable in the directory `build`.

## Running

First, ssh into the Raspberry Pi and copy the code `led_controller.py` into your current directory. Run `sudo python3 led_controller.py` to start the execution.

On Windows, run the programs with the following command, beginning in the directory that contains `CMakeLists.txt`.

```bash
build\Debug\fireflies | py experiments\\<experiment>.py
```

Note that this may not work correctly in PowerShell, so the normal Command Prompt is recommended.

On Mac or Linux, run the programs with the following command, beginning in the directory that contains `CMakeLists.txt`.

```bash
./build/fireflies | python3 experiments/<experiment>.py
```

## License and Credits

All the source code included publicly in this repository was written by Isaac Brown and Bowen Cheng, under the supervision of Guy Amichay and Daniel Abrams at Northwestern University. The project uses [OpenCV](https://opencv.org/) for retrieving camera input and creating a GUI preview, although the source code for OpenCV is not directly included in this repository.
