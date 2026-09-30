"""kivy-ios recipe for cffi: builds the `_cffi_backend` C extension that `vosk` needs."""
from kivy_ios.toolchain import CythonRecipe


class CffiRecipe(CythonRecipe):
    version = "1.17.1"
    url = "https://pypi.io/packages/source/c/cffi/cffi-{version}.tar.gz"
    depends = ["python3", "libffi"]
    python_depends = ["pycparser"]
    cythonize = False  # plain C extension, nothing to cythonize
    library = "libcffi.a"


recipe = CffiRecipe()
