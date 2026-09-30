[app]
title = VoskTest
package.name = vosktest
package.domain = org.krozark
source.dir = .
# No include_exts: buildozer applies it even to files matched by include_patterns, which would
# drop the model's .mdl/.fst/.conf files. Everything except the excluded folders is packaged.
source.exclude_dirs = android, ci, ios, bin
version = 1.0
requirements = python3,kivy,vosk
orientation = portrait
fullscreen = 0

# vosk = the Krozark/vosk-api fork (same Vosk version on every platform), see android/recipes/vosk
p4a.local_recipes = ./android/recipes

android.api = 34
android.minapi = 24
android.ndk = 25b
android.ndk_api = 24
android.archs = armeabi-v7a, arm64-v8a, x86_64, x86
android.accept_sdk_license = True
android.logcat_filters = *:S python:D

# `buildozer --profile emulator ...`: only the ABI of the CI emulator (faster)
[app@emulator]
android.archs = x86_64

[buildozer]
log_level = 2
warn_on_root = 1
