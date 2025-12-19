from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import sys

class CMakeBuild(build_ext):
    def run(self):
        # Use meson to build
        subprocess.check_call(['meson', 'setup', 'build', '--prefix=' + sys.prefix])
        subprocess.check_call(['meson', 'compile', '-C', 'build'])
        subprocess.check_call(['meson', 'install', '-C', 'build'])

setup(
    name="appwindows",
    version="1.2.0",
    packages=['appwindows'],
    package_dir={'': 'lib'},
    ext_modules=[Extension('appwindows', [])],  # Placeholder
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
)