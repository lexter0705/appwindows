import time
import subprocess

from .base import WindowCreator


class MacOSWindowCreator(WindowCreator):
    def __init__(self):
        self.__window_titles = []

    def __escape_applescript_string(self, text: str) -> str:
        escaped = text.replace('"', '\\"').replace('\\', '\\\\')
        return escaped

    def __run_applescript(self, script: str) -> tuple[str, str, int]:
        result = subprocess.run(
            ['osascript', '-e', script],
            capture_output=True,
            text=True
        )
        return result.stdout.strip(), result.stderr.strip(), result.returncode

    def create_window(self, title: str, width: int = 400, height: int = 300, x: int = 100, y: int = 100):
        escaped_title = self.__escape_applescript_string(title)

        script = f'''
        tell application "Finder"
            activate
            make new Finder window
            delay 0.5
            set allWindows to every window
            set targetWindow to item 1 of allWindows
            set name of targetWindow to "{escaped_title}"
            set bounds of targetWindow to {{{x}, {y}, {x + width}, {y + height}}}
        end tell
        '''

        self.__run_applescript(script)
        self.__window_titles.append(title)
        time.sleep(1)

    def cleanup(self):
        for title in self.__window_titles[:]:
            escaped_title = self.__escape_applescript_string(title)

            script = f'''
            tell application "Finder"
                set foundWindows to (every window whose name is "{escaped_title}")
                if (count of foundWindows) > 0 then
                    repeat with win in foundWindows
                        close win
                    end repeat
                end if
            end tell
            '''

            self.__run_applescript(script)
            self.__window_titles.remove(title)
