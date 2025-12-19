project('appwindows', 'cpp',
        license: 'BSD 3-Clause License',
        meson_version: '>=0.56.0',
        default_options: [
            'cpp_std=c++17',
            'buildtype=release',
            'default_library=shared',
            'b_ndebug=if-release',
            # Ключевые опции для Windows
            'b_vscrt=mt' if host_machine.system() == 'windows' else 'none',
        ]
)

# Проверка и настройка компилятора для Windows
cc = meson.get_compiler('cpp')
is_msvc = cc.get_id() == 'msvc'

if is_msvc
    # Для MSVC добавляем специфичные флаги
    add_project_arguments('/bigobj', language: 'cpp')
    add_project_arguments('/MP', language: 'cpp')  # Multi-processor compilation
    if get_option('buildtype') == 'release'
        add_project_arguments('/O2', '/Ob2', '/Oi', '/GL', language: 'cpp')
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

    # Windows библиотеки в правильном порядке
    win_libs = ['user32', 'gdi32', 'advapi32', 'shell32', 'dwmapi', 'ole32', 'oleaut32']

    foreach lib : win_libs
        dep = cc.find_library(lib, required: true)
        if dep.found()
            platform_deps += dep
            message('Found Windows library: ' + lib)
        endif
    endforeach

    # Для MSVC добавляем дополнительные флаги линковки
    if is_msvc
        platform_link_args = [
            '/NODEFAULTLIB:libcmt',
            '/DYNAMICBASE',
            '/NXCOMPAT',
            '/OPT:REF',
            '/OPT:ICF',
            '/LTCG',
        ]
    else
        platform_link_args = []
    endif

    # Проверяем наличие Windows SDK
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
                                      name_prefix: '',  # Важно для Windows
                                      name_suffix: 'pyd' if host_machine.system() == 'windows' else 'so',
subdir: 'appwindows')

# После сборки проверяем выходной файл
if host_machine.system() == 'windows'
    custom_target('verify-windows-build',
                  input: appwindows_mod,
                  output: 'verify.txt',
                  command: [
                      'cmd', '/c',
                      'echo', 'Windows module built successfully at', '@INPUT@',
                      '&&', 'dir', '/b', '@INPUT@',
                      '&&', 'echo', 'File exists check completed'
                  ],
                  build_by_default: true)
endif

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