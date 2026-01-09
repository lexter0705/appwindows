import sys

from .macos import MacOSWindowCreator
from .windows import WindowsWindowCreator
from .x_server import XServerWindowCreator
from .base import WindowCreator


def get_creator() -> WindowCreator:
    if sys.platform == "linux":
        return XServerWindowCreator()
    elif sys.platform == "darwin":
        return MacOSWindowCreator()
    else:
        return WindowsWindowCreator()
