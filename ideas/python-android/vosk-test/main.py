"""Minimal cross-platform check of the Vosk fork: transcribe data/test.wav and show the text.

Imports are at module level on purpose: a platform without a working `vosk` must fail at launch.
"""
import json
import os
import wave

from kivy.app import App
from kivy.uix.label import Label
from vosk import KaldiRecognizer, Model

DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "data")
CHUNK_FRAMES = 4000
RESULT_MARKER = "VOSK_TEST_RESULT:"  # read from the app output by the CI smoke tests


def transcribe(model_dir, wav_path):
    if not os.path.isdir(model_dir):
        parent = os.path.dirname(model_dir)
        found = os.listdir(parent) if os.path.isdir(parent) else "no such directory"
        raise FileNotFoundError(f"vosk model not found: {model_dir} (in {parent}: {found})")
    try:
        model = Model(model_dir)
    except Exception as error:
        # Kaldi only logs to stderr: report what the model folder really holds
        files = sorted(os.path.relpath(os.path.join(root, name), model_dir)
                       for root, _, names in os.walk(model_dir) for name in names)
        raise RuntimeError(f"{error}: {model_dir} holds {len(files)} files: {files}") from error
    with wave.open(wav_path, "rb") as wav:
        recognizer = KaldiRecognizer(model, wav.getframerate())
        while True:
            data = wav.readframes(CHUNK_FRAMES)
            if not data:
                break
            recognizer.AcceptWaveform(data)
    return json.loads(recognizer.FinalResult())["text"]


class VoskTestApp(App):
    def build(self):
        text = transcribe(os.path.join(DATA_DIR, "model"), os.path.join(DATA_DIR, "test.wav"))
        print(RESULT_MARKER, text, flush=True)
        return Label(text=text, halign="center", text_size=(300, None))


if __name__ == "__main__":
    VoskTestApp().run()
