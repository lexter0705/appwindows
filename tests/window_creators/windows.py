import time
from threading import Thread
from tkinter import Tk

from .base import WindowCreator


class WindowsWindowCreator(WindowCreator):
    def __init__(self):
        self.__processes = []
        self.__threads = []
        self.__windows = []

    def create_window(self, title, width=400, height=300, x=100, y=100):
        def run_window():
            window = Tk()
            window.title(title)
            window.geometry(f"{width}x{height}+{x}+{y}")
            self.__windows.append(window)
            window.update_idletasks()
            window.update()
            window.mainloop()

        thread = Thread(target=run_window, daemon=True)
        thread.start()
        self.__threads.append(thread)
        time.sleep(0.5)

    def cleanup(self):
        pass