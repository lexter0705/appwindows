import abc

from appwindows.base.point import Point


class Transform:
    @property
    @abc.abstractmethod
    def size(self) -> tuple:
        pass

    @size.setter
    @abc.abstractmethod
    def size(self, size: tuple):
        pass

    @property
    @abc.abstractmethod
    def points(self) -> list[Point]:
        pass

    @abc.abstractmethod
    def move(self, new_position: Point):
        pass