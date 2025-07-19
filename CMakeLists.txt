cmake_minimum_required(VERSION 3.31)

project(appwindows)

add_subdirectory(pybind11)

pybind11_add_module(appwindows
                    src/appwindows/core/bind.cc
                    src/appwindows/windows/bind.cc
                    src/appwindows/x_server/bind.cc
                    src/appwindows/bind.cc
                    src/appwindows/core/platform.cc
                    src/appwindows/core/geometry/point.cc
                    src/appwindows/core/geometry/size.cc
                    src/appwindows/windows/window.setters.cc
                    src/appwindows/windows/window.getters.cc
                    src/appwindows/windows/finder.cc
                    src/appwindows/x_server/finder.cc
                    src/appwindows/x_server/window.setters.cc
                    src/appwindows/x_server/window.getters.cc)

