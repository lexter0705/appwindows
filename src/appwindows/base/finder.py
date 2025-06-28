import abc

from appwindows.base.window import Window


class Finder(abc.ABC):
    @abc.abstractmethod
    def get_window_by_name(self, name: str) -> Window:
        pass

    @abc.abstractmethod
    def get_window_by_id(self, window_id: int) -> Window:
        pass