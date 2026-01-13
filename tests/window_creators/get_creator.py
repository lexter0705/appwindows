import sys

from .pyside6 import PySideWindowCreator
from .macos import MacOSWindowCreator
from .safari import SafariWindowCreator
from .windows import WindowsWindowCreator
from .x_server import XServerWindowCreator
from .base import WindowCreator


def get_creator() -> WindowCreator:
    if sys.platform == "linux":
        return XServerWindowCreator()
    elif sys.platform == "darwin":
        return PySideWindowCreator()
    else:
        return WindowsWindowCreator()
