import json
import logging
import os
import re
import wave


from kivy.app import App
from kivy.core.audio import SoundLoader
from kivy.lang import Builder
from kivy.uix.boxlayout import BoxLayout
from plyer.utils import platform

logger = logging.getLogger(__name__)

if platform == "android":
    from jnius import autoclass
    # Locale = autoclass('java.util.Locale')
    # TextToSpeech = autoclass('android.speech.tts.TextToSpeech')
    # PythonActivity = autoclass('org.kivy.android.PythonActivity')

    #Model = autoclass("org.vosk.Model")
    #Recognizer = autoclass('org.vosk.Recognizer')


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

Builder.load_string('''
<Speech2TextDemo>:
    BoxLayout:
        orientation: 'vertical'
        padding: 20

        TextInput:
            id: notification_text
            hint_text: 'logs'
        
        TextInput:
            id: path_text
            text: '.'
            hint_text: 'path'
        
        Button:
            text: 'list dir'
            size_hint_y: 0.2
            on_press: root.do_list_dir()
            
        Button:
            text: 'Read'
            size_hint_y: 0.2
            on_press: root.do_read()
        
        Button:
            text: 'STT'
            size_hint_y: 0.2
            on_press: root.do_stt()
''')


class Speech2TextDemo(BoxLayout):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def do_list_dir(self):
        p = self.ids.path_text.text
        print("== listing %s ==" % p)
        self.ids.notification_text.text += "\n== listing %s ==" % p
        for f in os.listdir(p):
            print(f)
            self.ids.notification_text.text += "\n" + str(f)

    def do_read(self):
        self.ids.notification_text.text += "\nreading data/sound.wav"

        sound = SoundLoader.load('data/sound.wav')

        if sound:
            print("Sound found at %s" % sound.source)
            print("Sound is %.3f seconds" % sound.length)
            sound.play()

    def do_stt(self):
        wf = wave.open('data/sound.wav', "rb")

        if wf.getnchannels() != 1 or wf.getsampwidth() != 2 or wf.getcomptype() != "NONE":
            print("Audio file must be WAV format mono PCM.")
            exit(1)

        model = Model("data/model")
        rec = Recognizer(model, wf.getframerate())

        while True:
            data = wf.readframes(4000)
            if len(data) == 0:
                break
            if rec.AcceptWaveform(data):
                res = json.loads(rec.Result())
                print(res)
                self.ids.notification_text.text += "\n" + res['text']
            else:
                res = json.loads(rec.PartialResult())
                print(res)
                self.ids.notification_text.text += "\n" + res['partial']

        res = rec.FinalResult()
        print(res)
        res = re.sub(r"([\d]+),([\d]+)", r"\1.\2", res) # replace X,YZ => X.YZ
        print(res)
        res = json.loads(res)
        print(res)
        self.ids.notification_text.text += "\n" + res['text']


class MainApp(App):
    def build(self):
        return Speech2TextDemo()

    def on_pause(self):
        return True


if __name__ == '__main__':
    app = MainApp()
    app.run()