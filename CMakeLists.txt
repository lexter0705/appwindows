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

set(LIBRARIES_TO_LINK)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    execute_process(
        COMMAND bash -c "loginctl show-session $(loginctl | grep $(whoami) | awk '{print $1}') -p Type"
        OUTPUT_VARIABLE result_display_server
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if ("${result_display_server}" MATCHES "Type=x11")
        find_package(X11 REQUIRED)
        add_compile_definitions(X_SERVER_USED NOMINMAX)
        list(APPEND PATH_TO_SOURCE
            src/appwindows/x_server/window.setters.cc
            src/appwindows/x_server/window.getters.cc
            src/appwindows/x_server/finder.cc)
        list(APPEND LIBRARIES_TO_LINK ${X11_LIBRARIES})
    elseif ("${result_display_server}" MATCHES "Type=wayland")
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


target_link_libraries(appwindows PRIVATE ${LIBRARIES_TO_LINK})