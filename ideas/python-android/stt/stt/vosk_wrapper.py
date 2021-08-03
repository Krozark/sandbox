import os

from plyer.utils import platform


if platform == "android":
    from ctypes import cdll
    _vosk_lib = cdll.LoadLibrary("libvosk.so")
    print("vosk lib: %s" % _vosk_lib)

if platform == "android":
    from jnius import autoclass
    # Locale = autoclass('java.util.Locale')
    # TextToSpeech = autoclass('android.speech.tts.TextToSpeech')
    # PythonActivity = autoclass('org.kivy.android.PythonActivity')

    Model = autoclass("org.vosk.Model")
    Recognizer = autoclass('org.vosk.Recognizer')
    Recognizer.AcceptWaveform = lambda self, data: self.acceptWaveForm(data, len(data))
    Recognizer.Result = lambda self: self.getResult()
    Recognizer.PartialResult = lambda self: self.getPartialResult()
    Recognizer.FinalResult = lambda self: self.getFinalResult()

    # import org.vosk.LibVosk;
    # import org.vosk.LogLevel;
    # import org.vosk.Model;
    # import org.vosk.Recognizer;
    # import org.vosk.android.RecognitionListener;
    # import org.vosk.android.SpeechService;
    # import org.vosk.android.SpeechStreamService;
    # import org.vosk.android.StorageService;
else:
    from vosk import (
        Model,
        KaldiRecognizer as Recognizer
    )
