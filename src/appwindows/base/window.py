import abc


class Window(abc.ABC):
    @abc.abstractmethod
    @property
    def size(self):
        pass

    @abc.abstractmethod
    @property
    def title(self):
        pass

    @abc.abstractmethod
    @property
    def id(self):
        pass

    @abc.abstractmethod
    def set_active(self):
        pass

    @abc.abstractmethod
    def move(self):
        pass

    @abc.abstractmethod
    def close(self):
        pass