"""kivy-ios recipe for the Krozark/vosk-api fork: Python binding + embedded libvosk.

libvosk is NOT built here: build it with vosk-api's `ios/build_libvosk.sh` and give the resulting
xcframework through LIBVOSK_XCFRAMEWORK. kivy-ios embeds it in the app's Frameworks folder
(`embed_xcframeworks`), where the binding finds it through the rpath.
"""
import os
import shutil
from os.path import join

import sh
from kivy_ios.context_managers import cd
from kivy_ios.toolchain import PythonRecipe, shprint


class VoskRecipe(PythonRecipe):
    version = "master"
    url = "https://github.com/Krozark/vosk-api/archive/{version}.zip"
    depends = ["python3", "cffi"]
    python_depends = ["requests", "tqdm", "srt", "websockets"]
    hostpython_prerequisites = ["cffi"]  # runs vosk_builder.py on the host
    embed_xcframeworks = ["libvosk"]

    def install(self):
        plat = list(self.platforms_to_build)[0]
        build_dir = self.get_build_dir(plat)

        # vosk/vosk_cffi.py is generated from src/vosk_api.h
        env = dict(os.environ, VOSK_SOURCE=build_dir)
        with cd(join(build_dir, "python")):
            shprint(sh.Command(self.ctx.hostpython), "vosk_builder.py", _env=env)
        shutil.copytree(
            join(build_dir, "python", "vosk"), join(self.ctx.site_packages_dir, "vosk"), dirs_exist_ok=True
        )

        # A missing LIBVOSK_XCFRAMEWORK is a build error on purpose (KeyError).
        xcframework = join(self.ctx.dist_dir, "xcframework", "libvosk.xcframework")
        shutil.rmtree(xcframework, ignore_errors=True)
        shutil.copytree(os.environ["LIBVOSK_XCFRAMEWORK"], xcframework)


recipe = VoskRecipe()
