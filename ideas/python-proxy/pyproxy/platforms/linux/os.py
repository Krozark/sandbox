from pyproxy.facades import Os

class LinuxOs(Os):
    def _name_impl(self):
        return "linux"


def instance_class():
    return LinuxOs