import abc


class Finder(abc.ABC):
    @abc.abstractmethod
    def get_window_by_tag(self):
        pass

    @abc.abstractmethod
    def get_window_by_id(self):
        pass