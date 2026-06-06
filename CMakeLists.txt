cmake_minimum_required(VERSION 3.30)
project(AcoustiQt VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Network Svg
LinguistTools)

qt_standard_project_setup()

add_subdirectory(application)
add_subdirectory(firmware)
