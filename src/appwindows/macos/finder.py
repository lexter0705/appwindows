from appwindows.base import Finder, Window


class MacOSFinder(Finder):
    def __init__(self):
        pass

    def get_window_by_name(self, name: str) -> Window:
        pass

    def get_window_by_id(self, window_id: int) -> Window:
        pass