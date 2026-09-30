#!/usr/bin/env bash
# Installs the APK on the running emulator, starts the app and checks the transcription it logs.
#
# Usage: ci/android_smoke_test.sh path/to/app.apk [out_dir]
set -euo pipefail

APK="${1:?usage: $0 path/to/app.apk [out_dir]}"
OUT_DIR="${2:-vosk-test-out}"
PACKAGE=org.krozark.vosktest
TIMEOUT="${TIMEOUT:-180}"
HERE="$(cd "$(dirname "$0")" && pwd)"

mkdir -p "$OUT_DIR"
adb install -r "$APK"
adb logcat -c
adb shell monkey -p "$PACKAGE" -c android.intent.category.LAUNCHER 1

for _ in $(seq "$TIMEOUT"); do
    adb logcat -d -s python:V > "$OUT_DIR/logcat.txt"
    grep -q "VOSK_TEST_RESULT:" "$OUT_DIR/logcat.txt" && break
    sleep 1
done
adb exec-out screencap -p > "$OUT_DIR/screenshot.png"

"$HERE/check_result.sh" < "$OUT_DIR/logcat.txt" || { tail -60 "$OUT_DIR/logcat.txt"; exit 1; }
