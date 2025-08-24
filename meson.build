project('appwindows', 'cpp',
        version : '0.1.0',
        license : 'MIT',
        meson_version : '>=0.56.0',
        default_options : [
            'cpp_std=c++17',
            'buildtype=release',
            'default_library=shared',
            'b_ndebug=if-release',
        ]
)

pybind11_dep = dependency('pybind11', required : true)
python_dep = dependency('python3', required : true)

py3 = import('python').find_installation('python3', pure: false)
python_install_dir = py3.get_install_dir()

common_inc = include_directories('src/appwindows/core')
base_inc = include_directories('src/appwindows')

# Общие файлы
common_sources = files(
    'src/appwindows/core/bind.cc',
    'src/appwindows/core/platform.cc',
    'src/appwindows/core/geometry/bind.cc',
    'src/appwindows/core/geometry/point.cc',
    'src/appwindows/core/geometry/size.cc',
)

# Файл модуля
module_sources = files('src/appwindows/appwindows.cc')

platform_sources = []
platform_deps = []
platform_args = []
platform_inc = [base_inc]

if host_machine.system() == 'windows'
    platform_sources = files(
        'src/appwindows/windows/window.getters.cc',
        'src/appwindows/windows/window.setters.cc',
        'src/appwindows/windows/finder.cc'
    )
    platform_inc += include_directories('src/appwindows/windows')
    platform_args = ['-DWINDOWS_USED=1']

elif host_machine.system() == 'linux' or host_machine.system() == 'darwin'
    # Сначала проверяем Wayland
    wayland_dep = dependency('wayland-client', required : false)

    if wayland_dep.found()
        platform_sources = files(
            'src/appwindows/wayland/window.getters.cc',
            'src/appwindows/wayland/window.setters.cc',
            'src/appwindows/wayland/finder.cc'
        )
        platform_inc += include_directories('src/appwindows/wayland')
        platform_deps += wayland_dep
        platform_args += ['-DWAYLAND_USED=1']

    else
        # Если Wayland не найден, пробуем X11
        x11_dep = dependency('x11', required : false)

        if x11_dep.found()
            platform_sources = files(
                'src/appwindows/x_server/window.getters.cc',
                'src/appwindows/x_server/window.setters.cc',
                'src/appwindows/x_server/finder.cc'
            )
            platform_inc += include_directories('src/appwindows/x_server')
            platform_deps += x11_dep
            platform_args += ['-DX_SERVER_USED=1']
        else
            warning('Neither Wayland nor X11 found, building without platform-specific features')
        endif
    endif

else
    error('Unsupported platform: ' + host_machine.system())
endif

all_sources = common_sources + module_sources + platform_sources
all_includes = [common_inc] + platform_inc

appwindows_module = py3.extension_module(
    'appwindows',
    all_sources,
    include_directories: all_includes,
    dependencies: [pybind11_dep, python_dep] + platform_deps,
    cpp_args: platform_args + ['-fvisibility=hidden'],
    install: true,
    subdir: 'appwindows'
)

configure_file(
    input: '__init__.py',
    output: '__init__.py',
    copy: true
)

install_data(
    '__init__.py',
    install_dir: python_install_dir / 'appwindows'
)