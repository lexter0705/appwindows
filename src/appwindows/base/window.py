import abc

from appwindows.base.transform import Transform


class Window(abc.ABC):
    @abc.abstractmethod
    def set_active(self, active: bool):
        pass

    @abc.abstractmethod
    def close(self):
        pass

    @property
    @abc.abstractmethod
    def title(self) -> str:
        pass

    @property
    @abc.abstractmethod
    def id(self) -> int:
        pass

    @property
    @abc.abstractmethod
    def transform(self) -> Transform:
        pass
