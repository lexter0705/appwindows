import time
import subprocess

from .base import WindowCreator


class XServerWindowCreator(WindowCreator):
    def __init__(self):
        self.__processes = []
        self.__threads = []
        self.__windows = []

    def create_window(self, title, width=400, height=300, x=100, y=100):
        cmd = [
            'yad',
            '--title', title,
            '--width', str(width),
            '--height', str(height),
            '--posx', str(x),
            '--posy', str(y),
            '--text', 'Test Window',
            '--button', 'Close:0',
            '--buttons-layout', 'center',
            '--geometry', f'{width}x{height}+{x}+{y}',
            '--undecorated=false',
            '--skip-taskbar',
            '--sticky',
            '--on-top',
            '--center'
        ]
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            start_new_session=True
        )
        self.__processes.append(proc)
        time.sleep(1)

    def cleanup(self):
        for proc in self.__processes:
            proc.terminate()
            proc.wait(timeout=1)
            if proc.poll() is None:
                proc.kill()

        subprocess.run(['pkill', '-f', 'yad'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
