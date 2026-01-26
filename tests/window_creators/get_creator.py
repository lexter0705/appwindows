import sys

from .base import WindowCreator


def get_creator() -> WindowCreator:
    if sys.platform == "linux":
        from .x_server import XServerWindowCreator
        return XServerWindowCreator()
    elif sys.platform == "darwin":
        from .pyside6 import PySideWindowCreator
        return PySideWindowCreator()
    else:
        from .windows import WindowsWindowCreator
        return WindowsWindowCreator()
