# vosk-test

Minimal Kivy app that checks the [Krozark/vosk-api](https://github.com/Krozark/vosk-api) fork
(Vosk `master`, one version on every platform) works on a platform: it transcribes `data/test.wav`
with a small English model and prints/shows the text.

`main.py` imports `vosk` at module level: a platform where Vosk cannot be loaded fails at launch.
Expected text (same as Linux with Vosk 0.3.45): `one zero zero zero one nine oh two i no zero one eight zero three`.

## Model

Not committed. Download it into `data/model`:

```bash
curl -fsSLO https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip
unzip -q vosk-model-small-en-us-0.15.zip && mv vosk-model-small-en-us-0.15 data/model
```

## iOS (kivy-ios)

- `ios/recipes/cffi`: `_cffi_backend` C extension, needed by `vosk` (kivy-ios has no recipe for it).
- `ios/recipes/vosk`: Python binding of the fork, plus `libvosk.xcframework` embedded in the app.
  libvosk comes from vosk-api's `ios/build_libvosk.sh` (path given in `LIBVOSK_XCFRAMEWORK`).
- `ios/smoke_test.sh`: runs the built app in an iOS Simulator and checks the transcription.

kivy-ios only lists its *bundled* recipes when it fills the Xcode project, so the recipes above are
copied into the installed `kivy_ios/recipes` before `toolchain build` (`--add-custom-recipe` builds them
but never links them into the app).

Built and run by `.github/workflows/vosk-test-ios.yml` (macOS runner, no Mac needed locally).
The screenshot and the logs are the `vosk-test-ios` artifact of the run.
