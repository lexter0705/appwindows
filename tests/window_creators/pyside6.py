import time
from multiprocessing import Process, Queue, Event
from PySide6.QtWidgets import QApplication, QMainWindow

from .base import WindowCreator


class PySideWindowCreator(WindowCreator):
    def __init__(self):
        self.__process = None
        self.__queue = Queue()
        self.__stop_event = Event()

    def create_window(self, title, width=400, height=300, x=100, y=100):
        if self.__process is None:
            self.__start_process()
            time.sleep(0.5)

        self.__queue.put({
            "title": title,
            "width": width,
            "height": height,
            "x": x,
            "y": y
        })

    def __start_process(self):
        self.__process = Process(
            target=self.run_qt_process,
            args=(self.__queue, self.__stop_event),
            daemon=True
        )
        self.__process.start()

    @staticmethod
    def run_qt_process(queue, stop_event):
        QApplication([])
        while not stop_event.is_set():
            if queue.empty():
                QApplication.processEvents()
                time.sleep(0.01)
                continue

            data = queue.get_nowait()
            window = QMainWindow()
            window.setWindowTitle(data["title"])
            window.setGeometry(data["x"], data["y"], data["width"], data["height"])
            window.show()

            QApplication.processEvents()

    def cleanup(self):
        if self.__process and self.__process.is_alive():
            self.__stop_event.set()
            self.__process.join(timeout=2.0)
            if self.__process.is_alive():
                self.__process.terminate()
            self.__process = None
