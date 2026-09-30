#!/usr/bin/env bash
# Downloads the small English Vosk model into data/model (run from the vosk-test folder).
set -euo pipefail

MODEL=vosk-model-small-en-us-0.15

curl -fsSL "https://alphacephei.com/vosk/models/$MODEL.zip" -o model.zip
python -m zipfile -e model.zip .
mv "$MODEL" data/model
rm model.zip
