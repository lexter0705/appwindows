import time
import subprocess
from threading import Thread

from .base import WindowCreator


class MacOSWindowCreator(WindowCreator):
    def __init__(self):
        self.__window_titles = []
        self.__processes = []

    def __escape_applescript_string(self, text: str) -> str:
        return text.replace('"', '\\"')

    def __create_window_async(self, script: str, title: str):
        process = subprocess.Popen(['osascript', '-e', script])
        self.__processes.append(process)
        self.__window_titles.append(title)

    def create_window(self, title: str, width: int = 400, height: int = 300,
                      x: int = 100, y: int = 100):
        escaped_title = self.__escape_applescript_string(title)
        script = f'''
        tell application "Terminal"
            activate
            do script ""
            delay 1
            set custom title of front window to "{escaped_title}"
            set bounds of front window to {{{x}, {y}, {x + width}, {y + height}}}
        end tell
        '''
        thread = Thread(target=self.__create_window_async, args=(script, title), daemon=True)
        thread.start()
        time.sleep(5)

    def cleanup(self):
        for title in self.__window_titles:
            escaped_title = self.__escape_applescript_string(title)
            script = f'''
            tell application "Terminal"
                close (every window whose custom title is "{escaped_title}")
            end tell
            '''
            subprocess.run(['osascript', '-e', script], capture_output=True)

        for process in self.__processes:
            if process.poll() is None:
                process.terminate()

        self.__processes.clear()
        self.__window_titles.clear()