import abc


class WindowCreator(abc.ABC):
    @abc.abstractmethod
    def create_window(self, title: str, width: int = 400, height: int = 300, x: int = 100, y: int = 100):
        pass

    @abc.abstractmethod
    def cleanup(self):
        pass
