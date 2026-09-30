#!/usr/bin/env bash
# Runs the app in a fresh iOS Simulator and checks the transcription it prints at startup.
# Needs macOS + Xcode. Screenshot and logs are left in <out_dir> for visual inspection.
#
# Usage: ios/smoke_test.sh path/to/App.app [out_dir]
set -euo pipefail

APP="${1:?usage: $0 path/to/App.app [out_dir]}"
OUT_DIR="${2:-vosk-test-out}"
TIMEOUT="${TIMEOUT:-180}"
MARKER="VOSK_TEST_RESULT:"  # printed by main.py
EXPECTED_PREFIX="one zero zero zero one nine oh two"  # start of the Linux reference transcription

mkdir -p "$OUT_DIR"
BUNDLE_ID=$(/usr/libexec/PlistBuddy -c "Print :CFBundleIdentifier" "$APP/Info.plist")
RUNTIME=$(xcrun simctl list runtimes -j | python3 -c \
  "import json,sys; print([r['identifier'] for r in json.load(sys.stdin)['runtimes'] if r['platform']=='iOS' and r['isAvailable']][-1])")

UDID=$(xcrun simctl create vosk-test "iPhone 15" "$RUNTIME")
trap 'xcrun simctl shutdown "$UDID" 2>/dev/null || true; xcrun simctl delete "$UDID"' EXIT
xcrun simctl boot "$UDID"
xcrun simctl bootstatus "$UDID" -b
xcrun simctl install "$UDID" "$APP"
xcrun simctl launch --stdout="$OUT_DIR/stdout.log" --stderr="$OUT_DIR/stderr.log" "$UDID" "$BUNDLE_ID"

result=""
for _ in $(seq "$TIMEOUT"); do
    result=$(cat "$OUT_DIR"/stdout.log "$OUT_DIR"/stderr.log 2>/dev/null | grep "$MARKER" | head -1 || true)
    [ -n "$result" ] && break
    sleep 1
done
xcrun simctl io "$UDID" screenshot "$OUT_DIR/screenshot.png"

if [ -z "$result" ]; then
    echo "::error::no '$MARKER' line after ${TIMEOUT}s (crash at launch, or vosk not loadable)"
    tail -60 "$OUT_DIR"/stdout.log "$OUT_DIR"/stderr.log || true
    exit 1
fi
echo "$result"
text="${result#*"$MARKER" }"
if [[ "$text" != "$EXPECTED_PREFIX"* ]]; then
    echo "::error::unexpected transcription: '$text' (expected to start with '$EXPECTED_PREFIX')"
    exit 1
fi
echo "OK: vosk transcribes correctly in the iOS Simulator"
