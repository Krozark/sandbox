"""Minimal cross-platform check of the Vosk fork: transcribe data/test.wav and show the text.

Imports are at module level on purpose: a platform without a working `vosk` must fail at launch.
"""
import json
import os
import wave

from kivy.app import App
from kivy.uix.label import Label
from vosk import KaldiRecognizer, Model, SetLogLevel

DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "data")
CHUNK_FRAMES = 4000
RESULT_MARKER = "VOSK_TEST_RESULT:"  # read from the app output by the CI smoke tests


def transcribe(model_dir, wav_path):
    SetLogLevel(-1)
    model = Model(model_dir)
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
