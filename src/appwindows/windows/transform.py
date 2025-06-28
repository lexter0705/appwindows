from appwindows.base.point import Point


class WindowsTransform:
    @property
    def size(self) -> tuple:
        pass

    @size.setter
    def size(self, size: tuple):
        pass

    @property
    def points(self) -> list[Point]:
        pass

    def move(self, new_position: Point):
        pass