import os
import sys
import subprocess
import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
            print(f"CMake version: {out.decode().strip()}")
        except OSError:
            raise RuntimeError("CMake must be installed to build extensions")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DPYTHON_EXECUTABLE={sys.executable}',
            '-DCMAKE_BUILD_TYPE=Release'
        ]

        # Platform specific settings
        if platform.system() == "Windows":
            cmake_args += [
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE={}'.format(extdir),
                '-G', 'Visual Studio 17 2022'  # Adjust based on your VS version
            ]
        else:
            cmake_args += ['-G', 'Ninja' if self._has_ninja() else 'Unix Makefiles']

        # Build arguments
        build_args = ['--config', 'Release']

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        # Run CMake configure
        print("Running CMake configure...")
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp)

        # Run CMake build
        print("Running CMake build...")
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)

    def _has_ninja(self):
        try:
            subprocess.check_output(['ninja', '--version'])
            return True
        except (OSError, subprocess.CalledProcessError):
            return False

setup(
    name="appwindows",
    version="1.0.0",
    author=[{"name": "lexter0705", "email": "lexter0705@gmail.com"}],
    description="The appwindows library solves the problem of working with a graphical shell on different operating systems.",
    readme="README.md",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
        "Operating System :: Windows, Linux",
    ],
    ext_modules=[CMakeExtension('appwindows')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    python_requires='>=3.6',
)
