"""python-for-android recipe for the Krozark/vosk-api fork: Python binding + libvosk.so.

Same version as every other platform (the fork's master), not the PyPI/Maven 0.3.45 of the stock
recipe. libvosk.so is NOT built here: build it with the fork's android/lib/build-vosk.sh and give
the folder holding `<abi>/libvosk.so` through LIBVOSK_ANDROID_DIR.
"""
import os
import shutil
from os.path import join

from pythonforandroid.logger import info
from pythonforandroid.recipe import PythonRecipe, current_directory, shprint
from pythonforandroid.util import ensure_dir


class VoskRecipe(PythonRecipe):
    version = "master"
    url = "https://github.com/Krozark/vosk-api/archive/{version}.zip"
    site_packages_name = "vosk"
    depends = ["cffi"]
    python_depends = ["requests", "tqdm", "srt", "websockets"]
    hostpython_prerequisites = ["setuptools", "wheel", "cffi", "requests", "tqdm", "srt", "websockets"]
    call_hostpython_via_targetpython = False

    def build_arch(self, arch):
        self.install_hostpython_prerequisites()

        env = self.get_recipe_env(arch)
        python_dir = join(self.get_build_dir(arch.arch), "python")
        install_dir = self.ctx.get_python_install_dir(arch.arch)

        info("Installing the Vosk Python binding into site-packages")
        with current_directory(python_dir):
            shprint(
                self._host_recipe.pip,
                "install",
                ".",
                "--compile",
                "--no-deps",
                "--target",
                install_dir,
                _env=env,
            )

        # A missing LIBVOSK_ANDROID_DIR or ABI is a build error on purpose (KeyError, no such file).
        source = join(os.environ["LIBVOSK_ANDROID_DIR"], arch.arch, "libvosk.so")
        target = join(install_dir, self.site_packages_name, "libvosk.so")
        ensure_dir(os.path.dirname(target))
        info(f"Installing libvosk.so for {arch.arch}")
        shutil.copyfile(source, target)


recipe = VoskRecipe()
