cmake_minimum_required(VERSION 3.31)

project(appwindows)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(pybind11_DIR ./.venv/Lib/site-packages/pybind11/share/cmake)

include(FetchContent)
FetchContent_Declare(
        pybind11
        URL https://github.com/pybind/pybind11/archive/refs/tags/v2.10.0.zip
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(appwindows src/appwindows/appwindows.cc)

