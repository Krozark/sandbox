#!/usr/bin/env bash
# Checks the output of the app (stdin): it must hold the VOSK_TEST_RESULT line printed by main.py,
# with the reference transcription. One definition of the expected text for every platform.
#
# The expected start is what Linux gives with the same model and audio (Vosk 0.3.45 and master).
set -euo pipefail

MARKER="VOSK_TEST_RESULT:"
EXPECTED_PREFIX="one zero zero zero one nine oh two"

result="$(grep "$MARKER" | head -1 || true)"
if [ -z "$result" ]; then
    echo "::error::no '$MARKER' line in the app output (crash at launch, or vosk not loadable)"
    exit 1
fi
echo "$result"
text="${result#*"$MARKER" }"
if [[ "$text" != "$EXPECTED_PREFIX"* ]]; then
    echo "::error::unexpected transcription: '$text' (expected to start with '$EXPECTED_PREFIX')"
    exit 1
fi
echo "OK: vosk transcribes correctly"
