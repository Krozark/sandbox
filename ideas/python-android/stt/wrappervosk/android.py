import os

from cffi import FFI

header_file = os.path.join(os.path.abspath(os.path.dirname(__file__)), "..", "data", "vosk", "vosk_api.h")

_ffi = FFI()
with open(header_file, "r") as f:
    _ffi.cdef(f.read())
_vosk_lib = _ffi.dlopen("libvosk.so")
#
# from ctypes import cdll
# _vosk_lib = cdll.LoadLibrary()
# print("vosk lib: %s" % _vosk_lib)


class Model(object):

    def __init__(self, model_path):
        self._handle = _vosk_lib.vosk_model_new(model_path.encode('utf-8'))

    def __del__(self):
        _vosk_lib.vosk_model_free(self._handle)

    def vosk_model_find_word(self, word):
        return _vosk_lib.vosk_model_find_word(self._handle, word.encode('utf-8'))


class SpkModel(object):

    def __init__(self, model_path):
        self._handle = _vosk_lib.vosk_spk_model_new(model_path.encode('utf-8'))

    def __del__(self):
        _vosk_lib.vosk_spk_model_free(self._handle)


class KaldiRecognizer(object):

    def __init__(self, *args):
        if len(args) == 2:
            self._handle = _vosk_lib.vosk_recognizer_new(args[0]._handle, args[1])
        elif len(args) == 3 and type(args[2]) is SpkModel:
            self._handle = _vosk_lib.vosk_recognizer_new_spk(args[0]._handle, args[1], args[2]._handle)
        elif len(args) == 3 and type(args[2]) is str:
            self._handle = _vosk_lib.vosk_recognizer_new_grm(args[0]._handle, args[1], args[2].encode('utf-8'))
        else:
            raise TypeError("Unknown arguments")

    def __del__(self):
        _vosk_lib.vosk_recognizer_free(self._handle)

    def SetMaxAlternatives(self, max_alternatives):
        _vosk_lib.vosk_recognizer_set_max_alternatives(self._handle, max_alternatives)

    def SetWords(self, enable_words):
        _vosk_lib.vosk_recognizer_set_words(self._handle, 1 if enable_words else 0)

    def SetSpkModel(self, spk_model):
        _vosk_lib.vosk_recognizer_set_spk_model(self._handle, spk_model._handle)

    def AcceptWaveform(self, data):
        return _vosk_lib.vosk_recognizer_accept_waveform(self._handle, data, len(data))

    def Result(self):
        return _ffi.string(_vosk_lib.vosk_recognizer_result(self._handle)).decode('utf-8')

    def PartialResult(self):
        return _ffi.string(_vosk_lib.vosk_recognizer_partial_result(self._handle)).decode('utf-8')

    def FinalResult(self):
        return _ffi.string(_vosk_lib.vosk_recognizer_final_result(self._handle)).decode('utf-8')

    def Reset(self):
        return _vosk_lib.vosk_recognizer_reset(self._handle)


def SetLogLevel(level):
    return _vosk_lib.vosk_set_log_level(level)


def GpuInit():
    _vosk_lib.vosk_gpu_init()


def GpuThreadInit():
    _vosk_lib.vosk_gpu_thread_init()

# if platform == "android":
#
#     from jnius import autoclass
#     # Locale = autoclass('java.util.Locale')
#     # TextToSpeech = autoclass('android.speech.tts.TextToSpeech')
#     # PythonActivity = autoclass('org.kivy.android.PythonActivity')
#
#     Model = autoclass("org.vosk.Model")
#     Recognizer = autoclass('org.vosk.Recognizer')
#     Recognizer.AcceptWaveform = lambda self, data: self.acceptWaveForm(data, len(data))
#     Recognizer.Result = lambda self: self.getResult()
#     Recognizer.PartialResult = lambda self: self.getPartialResult()
#     Recognizer.FinalResult = lambda self: self.getFinalResult()
#
#     # import org.vosk.LibVosk;
#     # import org.vosk.LogLevel;
#     # import org.vosk.Model;
#     # import org.vosk.Recognizer;
#     # import org.vosk.android.RecognitionListener;
#     # import org.vosk.android.SpeechService;
#     # import org.vosk.android.SpeechStreamService;
#     # import org.vosk.android.StorageService;
