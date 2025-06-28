class Size:
    def __init__(self, width, height):
        self.__width = width
        self.__height = height

    def set_size(self, width, height):
        if width <= 1 or height <= 1:
            raise Exception("Invalid size")

        self.__width = width
        self.__height = height

    @property
    def width(self):
        return self.__width

    @property
    def height(self):
        return self.__height
