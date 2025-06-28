class Point:
    def __init__(self, x, y):
        if not isinstance(x, int) or not isinstance(y, int):
            raise TypeError(f"x and y must be integers, not {type(x)} and {type(y)}")

        self.__x = x
        self.__y = y

    @property
    def x(self):
        return self.__x

    @property
    def y(self):
        return self.__y

    def __add__(self, other: 'Point'):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other: 'Point'):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, other: 'Point'):
        return Point(self.x * other.x, self.y * other.y)

    def __truediv__(self, other: 'Point'):
        return Point(self.x / other.x, self.y / other.y)
