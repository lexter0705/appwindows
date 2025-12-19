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

if host_machine.system() == 'windows'
    appwindows_sources += files(
        'src/windows/window.getters.cc',
        'src/windows/window.setters.cc',
        'src/windows/finder.cc'
    )

    platform_args = [
        '-DWINDOWS_USED=1',
        '-DNOMINMAX',
        '-D_CRT_SECURE_NO_WARNINGS'
    ]

    # Windows библиотеки - простой способ
    platform_deps += [
        meson.get_compiler('cpp').find_library('user32'),
        meson.get_compiler('cpp').find_library('gdi32'),
        meson.get_compiler('cpp').find_library('advapi32'),
        meson.get_compiler('cpp').find_library('shell32'),
    ]

elif host_machine.system() == 'linux'
    x11_dep = dependency('x11', required: true)
    appwindows_sources += files(
        'src/x_server/window.getters.cc',
        'src/x_server/window.setters.cc',
        'src/x_server/finder.cc'
    )
    platform_deps += [x11_dep]
    platform_args = ['-DX_SERVER_USED=1']

else
    error('Unsupported platform: ' + host_machine.system())
endif

appwindows_inc = include_directories(
    'src/',
    'src/core',
    'src/core/geometry',
    'src/core/exceptions'
)

if host_machine.system() == 'windows'
    appwindows_inc = [appwindows_inc, include_directories('src/windows')]
elif host_machine.system() == 'linux'
    appwindows_inc = [appwindows_inc, include_directories('src/x_server')]
endif

appwindows_mod = py3.extension_module('appwindows',
                                      sources: appwindows_sources,
                                      dependencies: [pybind11_dep, python_dep] + platform_deps,
                                      include_directories: appwindows_inc,
                                      install: true,
                                      cpp_args: platform_args,
                                      subdir: 'appwindows')

# Install Python files
install_subdir('lib',
               install_dir: py3.get_install_dir(),
               strip_directory: true)