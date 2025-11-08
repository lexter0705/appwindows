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

pybind11_dep = dependency('pybind11', required: true)
python_dep = dependency('python3', required: true)

py3 = import('python').find_installation('python3', pure: false)
python_install_dir = py3.get_install_dir()

appwindows_inc = include_directories(
    'src/appwindows',
    'src/appwindows/core'
)
geometry_inc = include_directories('src/appwindows/core/geometry')
exceptions_inc = include_directories('src/appwindows/core/exceptions')

appwindows_sources = files(
    'src/appwindows/appwindows.cc',
    'src/appwindows/core/bind.cc',
    'src/appwindows/core/platform.cc',
    'src/appwindows/core/geometry/size.cc',
    'src/appwindows/core/geometry/point.cc',
    'src/appwindows/core/geometry/bind_geometry.cc',
    'src/appwindows/core/exceptions/bind_exceptions.cc'
)

platform_deps = []
platform_args = []

if host_machine.system() == 'windows'
    appwindows_sources += files(
        'src/appwindows/windows/window.getters.cc',
        'src/appwindows/windows/window.setters.cc',
        'src/appwindows/windows/finder.cc'
    )
    appwindows_inc = [appwindows_inc, include_directories('src/appwindows/windows')]
    platform_args = ['-DWINDOWS_USED=1']

elif host_machine.system() == 'linux'
    x11_dep = dependency('x11', required: false)
    is_x11 = x11_dep.found()
    if is_x11
        appwindows_sources += files(
            'src/appwindows/x_server/window.getters.cc',
            'src/appwindows/x_server/window.setters.cc',
            'src/appwindows/x_server/finder.cc'
        )
        appwindows_inc = [appwindows_inc, include_directories('src/appwindows/x_server')]
        platform_deps += [x11_dep]
        platform_args += ['-DX_SERVER_USED=1']
    else
        error('For succsesful compile need x11 library')
    endif

elif host_machine.system() == 'darwin'
    appwindows_sources += files(
        'src/appwindows/macos/window.getters.cc',
        'src/appwindows/macos/window.setters.cc',
        'src/appwindows/macos/finder.cc'
    )
    appwindows_inc = [appwindows_inc, include_directories('src/appwindows/macos')]
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