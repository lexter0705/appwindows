import time
import sys
import subprocess
from threading import Thread

if sys.platform != "linux":
    from tkinter import Tk


class WindowCreator:
    def __init__(self):
        self.__processes = []
        self.__threads = []
        self.__windows = []

    def create_window(self, title, width=400, height=300, x=100, y=100):
        if sys.platform == "linux":
            return self.__create_xterm_window(title, width, height, x, y)
        else:
            return self.__create_tkinter_window(title, width, height, x, y)

    def __create_xterm_window(self, title, width, height, x, y):
        cmd = [
            'xterm',
            '-title', title,
            '-geometry', f'{width}x{height}+{x}+{y}',
            '-bg', 'white',
            '-fg', 'black',
            '-e', 'sleep 3600'
        ]

        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            start_new_session=True
        )

        self.__processes.append(proc)
        time.sleep(2)

        if proc.poll() is None:
            return True

        stdout, stderr = proc.communicate()
        return False

    def __create_tkinter_window(self, title, width, height, x, y):
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
        return True

    def cleanup(self):
        if sys.platform == "linux":
            for proc in self.__processes:
                proc.terminate()
                proc.wait(timeout=2)
                if proc.poll() is None:
                    proc.kill()

            subprocess.run(['pkill', '-f', 'xterm.*sleep'], timeout=3)
        else:
            for window in self.__windows:
                window.quit()
                window.destroy()

            for thread in self.__threads:
                if thread.is_alive():
                    thread.join(timeout=1)

        self.__processes.clear()
        self.__threads.clear()
        self.__windows.clear()
