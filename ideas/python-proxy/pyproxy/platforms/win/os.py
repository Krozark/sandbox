from pyproxy.facades import Os

class WinOs(Os):
    def _name_impl(self):
        return "win"


def instance_class():
    return WinOs