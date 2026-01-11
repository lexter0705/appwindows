import subprocess
import time

from .base import WindowCreator


class SafariWindowCreator(WindowCreator):
    def __init__(self):
        self.__window_titles = []

    def __escape_applescript_string(self, text: str) -> str:
        return text.replace('"', '\\"')

    def __run_applescript(self, script: str):
        subprocess.run(['osascript', '-e', script], capture_output=True)

    def create_window(self, title: str, width: int = 400, height: int = 300,
                      x: int = 100, y: int = 100):
        escaped_title = self.__escape_applescript_string(title)

        script = f'''
        tell application "Safari"
            activate
            make new document
            set custom title of front window to "{escaped_title}"
            set bounds of front window to {{{x}, {y}, {x + width}, {y + height}}}
        end tell
        '''

        self.__run_applescript(script)
        self.__window_titles.append(title)
        time.sleep(2)

    def cleanup(self):
        for title in self.__window_titles:
            escaped_title = self.__escape_applescript_string(title)
            script = f'''
            tell application "Safari"
                close (every window whose custom title is "{escaped_title}")
            end tell
            '''
            self.__run_applescript(script)
        self.__window_titles.clear()
