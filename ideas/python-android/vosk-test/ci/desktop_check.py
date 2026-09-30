"""Desktop check (Linux, Windows): transcribe data/test.wav with the installed vosk, like main.py."""
import os
import sys

APP_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
sys.path.insert(0, APP_DIR)

import main  # noqa: E402  (needs APP_DIR on sys.path; imports vosk at module level)

data_dir = os.path.join(APP_DIR, "data")
print(main.RESULT_MARKER, main.transcribe(os.path.join(data_dir, "model"), os.path.join(data_dir, "test.wav")))
