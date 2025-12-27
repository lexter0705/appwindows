import time
import tkinter as tk
from threading import Thread

import numpy as np
import pytest

from appwindows import get_finder
from appwindows.geometry import Point, Size


class TestWindowManager:
    def __init__(self):
        self.windows = []
        self.threads = []
        
    def create_window(self, title, width=400, height=300, x=100, y=100):
        def run_window():
            root = tk.Tk()
            root.title(title)
            root.geometry(f"{width}x{height}+{x}+{y}")
            root.update_idletasks()
            root.update()
            self.windows.append(root)
            root.mainloop()
            
        thread = Thread(target=run_window, daemon=True)
        thread.start()
        self.threads.append(thread)
        time.sleep(0.5)
        return thread
    
    def create_multiple_windows(self, titles):
        threads = []
        for i, title in enumerate(titles):
            thread = self.create_window(title, x=100 + i*50, y=100 + i*50)
            threads.append(thread)
        return threads
    
    def cleanup(self):
        for window in self.windows:
            try:
                window.destroy()
            except:
                pass
        self.windows = []
        self.threads = []

@pytest.fixture
def window_manager():
    manager = TestWindowManager()
    yield manager
    manager.cleanup()

@pytest.fixture
def finder():
    return get_finder()

@pytest.fixture
def test_window(window_manager, finder):
    window_manager.create_window("Test Window for Operations")
    time.sleep(1)
    
    window = finder.get_window_by_title("Test Window for Operations")
    assert window is not None
    return window

def test_window_get_title(test_window):
    title = test_window.get_title()
    assert isinstance(title, str)
    assert "Test Window for Operations" in title

def test_window_get_points(test_window):
    quad_points = test_window.get_points()
    assert hasattr(quad_points, 'left_top')
    assert hasattr(quad_points, 'right_top')
    assert hasattr(quad_points, 'right_bottom')
    assert hasattr(quad_points, 'left_bottom')
    
    assert isinstance(quad_points.left_top, Point)
    assert isinstance(quad_points.right_top, Point)
    assert isinstance(quad_points.right_bottom, Point)
    assert isinstance(quad_points.left_bottom, Point)

def test_window_get_size(test_window):
    size = test_window.get_size()
    assert isinstance(size, Size)
    assert size.width > 0
    assert size.height > 0

def test_window_get_screenshot(test_window):
    screenshot = test_window.get_screenshot()
    assert screenshot is not None
    assert isinstance(screenshot, np.ndarray)
    assert len(screenshot.shape) >= 2

def test_window_move(test_window):
    current_points = test_window.get_points()
    current_x = current_points.left_top.x
    current_y = current_points.left_top.y
    
    new_point = Point(current_x + 50, current_y + 50)
    test_window.move(new_point)
    
    time.sleep(0.5)
    updated_points = test_window.get_points()
    assert updated_points.left_top.x == current_x + 50
    assert updated_points.left_top.y == current_y + 50

def test_window_resize(test_window):
    current_size = test_window.get_size()
    new_size = Size(current_size.width + 100, current_size.height + 50)
    test_window.resize(new_size)
    
    time.sleep(0.5)
    updated_size = test_window.get_size()
    assert updated_size.width == current_size.width + 100
    assert updated_size.height == current_size.height + 50

def test_window_minimize_restore(test_window):
    test_window.set_minimize(True)
    time.sleep(1)
    
    test_window.set_minimize(False)
    time.sleep(1)

def test_window_fullscreen(test_window):
    test_window.set_fullscreen(True)
    time.sleep(1)
    
    test_window.set_fullscreen(False)
    time.sleep(1)

def test_window_to_foreground_background(test_window):
    test_window.to_foreground()
    time.sleep(0.5)
    
    test_window.to_background()
    time.sleep(0.5)

def test_window_close(test_window, window_manager):
    test_window.close()
    time.sleep(1)
    
    finder = get_finder()
    with pytest.raises(Exception):
        finder.get_window_by_title("Test Window for Operations")