from screeninfo import get_monitors

from src.appwindows.base.size.base import Size


class MaxSize(Size):
    def __init__(self):
        monitor = get_monitors()[0]
        super().__init__(monitor.width, monitor.height)
