cmake_minimum_required(VERSION 3.25)

project(appwindows)

add_subdirectory(pybind11)

set(PATH_TO_SOURCE
    src/appwindows/appwindows.cc
    src/appwindows/core/bind.cc
    src/appwindows/core/platform.cc
    src/appwindows/core/geometry/bind.cc
    src/appwindows/core/geometry/point.cc
    src/appwindows/core/geometry/size.cc)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if ("${_display_server_type}" MATCHES "Type=x11")
        add_compile_definitions(X_SERVER_USED NOMINMAX)
        list(APPEND PATH_TO_SOURCE
            src/appwindows/x_server/window.setters.cc
            src/appwindows/x_server/window.getters.cc
            src/appwindows/x_server/finder.cc)
        define_property(X_SERVER_USE)
    elseif ("${_display_server_type}" MATCHES "Type=wayland")
        add_compile_definitions(WAYLAND_USED NOMINMAX)
        list(APPEND PATH_TO_SOURCE
            src/appwindows/wayland/window.setters.cc
            src/appwindows/wayland/window.getters.cc
            src/appwindows/wayland/finder.cc)
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(WINDOWS_USED NOMINMAX)
    list(APPEND PATH_TO_SOURCE
        src/appwindows/windows/window.setters.cc
        src/appwindows/windows/window.getters.cc
        src/appwindows/windows/finder.cc)
endif ()

pybind11_add_module(appwindows
                    ${PATH_TO_SOURCE})

