project('appwindows', 'cpp',
        license: 'BSD 3-Clause License',
        meson_version: '>=0.56.0',
        default_options: [
            'cpp_std=c++17',
            'buildtype=release',
            'default_library=shared',
            'b_ndebug=if-release',
        ]
)

cc = meson.get_compiler('cpp')
is_msvc = cc.get_id() == 'msvc'

if host_machine.system() == 'windows'
    if is_msvc
        add_global_arguments('/MT', language: 'cpp')
        add_global_link_arguments('/NODEFAULTLIB:libcmt', language: 'cpp')
    endif
endif

pybind11_dep = dependency('pybind11', required: true)
python_dep = dependency('python3', required: true)

py3 = import('python').find_installation('python3', pure: false)
python_install_dir = py3.get_install_dir()

appwindows_inc = include_directories(
    'src/',
    'src/core'
)
geometry_inc = include_directories('src/core/geometry')
exceptions_inc = include_directories('src/core/exceptions')

appwindows_sources = files(
    'src/appwindows.cc',
    'src/core/bind.cc',
    'src/core/platform.cc',
    'src/core/geometry/size.cc',
    'src/core/geometry/point.cc',
    'src/core/geometry/bind_geometry.cc',
    'src/core/exceptions/bind_exceptions.cc'
)

platform_deps = []
platform_args = []
platform_link_args = []

if host_machine.system() == 'windows'
    message('Building for Windows')

    appwindows_sources += files(
        'src/windows/window.getters.cc',
        'src/windows/window.setters.cc',
        'src/windows/finder.cc'
    )
    appwindows_inc = [appwindows_inc, include_directories('src/windows')]
    platform_args = ['-DWINDOWS_USED=1', '-DNOMINMAX', '-D_CRT_SECURE_NO_WARNINGS']

    win_libs = ['user32', 'gdi32', 'advapi32', 'shell32', 'dwmapi', 'ole32', 'oleaut32']

    foreach lib : win_libs
        dep = cc.find_library(lib, required: false)
        if dep.found()
            platform_deps += dep
            message('Found Windows library: ' + lib)
        else
            message('Warning: Windows library not found: ' + lib)
        endif
    endforeach

    if is_msvc
        platform_args += ['/bigobj', '/MP']
        platform_link_args = [
            '/DYNAMICBASE',
            '/NXCOMPAT',
            '/OPT:REF',
            '/OPT:ICF',
        ]
        if get_option('buildtype') == 'release'
            platform_args += ['/O2', '/Ob2', '/Oi', '/GL']
            platform_link_args += ['/LTCG']
        endif
    endif

    message('Checking for Windows headers...')
    if cc.has_header('windows.h')
        message('Windows.h found')
    else
        error('Windows.h not found!')
    endif

elif host_machine.system() == 'linux'
    x11_dep = dependency('x11', required: false)
    is_x11 = x11_dep.found()
    if is_x11
        appwindows_sources += files(
            'src/x_server/window.getters.cc',
            'src/x_server/window.setters.cc',
            'src/x_server/finder.cc'
        )
        appwindows_inc = [appwindows_inc, include_directories('src/x_server')]
        platform_deps += [x11_dep]
        platform_args += ['-DX_SERVER_USED=1']
    else
        error('For successful compile need x11 library')
    endif

elif host_machine.system() == 'darwin'
    appwindows_sources += files(
        'src/macos/window.getters.cc',
        'src/macos/window.setters.cc',
        'src/macos/finder.cc'
    )
    appwindows_inc = [appwindows_inc, include_directories('src/macos')]
    platform_args += ['-DMACOS_USED=1']

else
    error('Unsupported platform: ' + host_machine.system())
endif

appwindows_mod = py3.extension_module('appwindows',
                                      sources: appwindows_sources,
                                      dependencies: [pybind11_dep, python_dep] + platform_deps,
                                      include_directories: [appwindows_inc, exceptions_inc, geometry_inc],
                                      install: true,
                                      cpp_args: platform_args,
                                      link_args: platform_link_args,
                                      subdir: 'appwindows')

configure_file(
    input: 'lib/__init__.py',
    output: '__init__.py',
    configuration: configuration_data(),
    install: true,
    install_dir: python_install_dir / 'appwindows'
)

install_data(
    'lib/geometry/__init__.py',
    install_dir: python_install_dir / 'appwindows/geometry'
)

install_data(
    'lib/exceptions/__init__.py',
    install_dir: python_install_dir / 'appwindows/exceptions'
)