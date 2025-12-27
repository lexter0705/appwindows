import time
import tkinter as tk
from threading import Thread

import pytest

from appwindows import get_finder
from appwindows.exceptions import WindowDoesNotFoundException


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

def test_get_all_windows(window_manager, finder):
    window_manager.create_window("Test Window 1")
    time.sleep(1)
    
    windows = finder.get_all_windows()
    assert isinstance(windows, list)
    assert len(windows) > 0
    
    found = False
    for window in windows:
        if "Test Window 1" in window.get_title():
            found = True
            break
    assert found

def test_get_all_titles(window_manager, finder):
    titles = ["Window A", "Window B", "Window C"]
    window_manager.create_multiple_windows(titles)
    time.sleep(1)
    
    all_titles = finder.get_all_titles()
    assert isinstance(all_titles, list)
    assert len(all_titles) >= len(titles)
    
    for title in titles:
        assert any(title in t for t in all_titles)

def test_get_window_by_title(window_manager, finder):
    window_manager.create_window("Target Window")
    time.sleep(1)
    
    window = finder.get_window_by_title("Target Window")
    assert window is not None
    assert "Target Window" in window.get_title()

def test_get_window_by_title_substring(window_manager, finder):
    window_manager.create_window("Main Application Window")
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