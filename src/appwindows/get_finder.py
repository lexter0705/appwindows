import platform

from appwindows.macos import MacOSFinder
from appwindows.windows import WindowsFinder
from appwindows.x_server import XServerFinder


def get_finder():
    os_name = platform.system()
    if os_name == "Windows":
        return WindowsFinder()
    if os_name == "Darwin":
        return MacOSFinder()
    if os_name == "Linux":
        return XServerFinder()
    raise NotImplementedError(f"{os_name} is not supported")
