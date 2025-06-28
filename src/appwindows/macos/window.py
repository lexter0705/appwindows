from appwindows.base import Window, Transform


class MacOsWindow(Window):
    def set_active(self, active: bool):
        pass

    def close(self):
        pass

    @property
    def title(self) -> str:
        pass

    @property
    def id(self) -> int:
        pass

    @property
    def transform(self) -> Transform:
        pass
