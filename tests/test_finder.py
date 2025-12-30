import time

from tkinter import Tk
from threading import Thread

from PySide6.QtWidgets import QApplication, QMainWindow, QLabel
from PySide6.QtCore import Qt, QTimer

import pytest

from appwindows import get_finder
from appwindows.exceptions import WindowDoesNotFoundException
from appwindows.geometry import Size

from window_creator import WindowCreator


@pytest.fixture
def finder():
    return get_finder()


def test_get_all_windows(finder):
    window_creator = WindowCreator()
    window_creator.create_window("Test Window 1")
    time.sleep(1)

    windows = finder.get_all_windows()
    assert isinstance(windows, list)
    assert len(windows) > 0
    print("Windows getted!")
    found = False
    for window in windows:
        if "Test Window 1" in window.get_title():
            found = True
            break
    assert found


def test_get_all_titles(finder):
    creator = WindowCreator()
    titles = ["Window A", "Window B", "Window C"]
    for i in titles:
        creator.create_window(i)

    time.sleep(1)

    all_titles = finder.get_all_titles()
    assert isinstance(all_titles, list)
    assert len(all_titles) >= len(titles)

    for title in titles:
        assert any(title in t for t in all_titles)


def test_get_window_by_title(finder):
    window_creator = WindowCreator()
    window_creator.create_window("Target Window")
    time.sleep(1)

    window = finder.get_window_by_title("Target Window")
    assert window is not None
    assert "Target Window" in window.get_title()


def test_get_window_by_title_substring(finder):
    window_creator = WindowCreator()
    window_creator.create_window("Main Application Window")
    time.sleep(1)

    window = finder.get_window_by_title("Application")
    assert window is not None
    assert "Main Application Window" in window.get_title()


def test_get_window_by_title_not_found(finder):
    with pytest.raises(WindowDoesNotFoundException):
        finder.get_window_by_title("NonExistentWindowTitle12345")


def test_get_os_info(finder):
    os_info = finder.get_os()
    assert isinstance(os_info, str)
    assert len(os_info) > 0
