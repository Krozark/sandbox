
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.boxlayout import BoxLayout

from plyer.utils import platform

from jnius import autoclass

Locale = autoclass('java.util.Locale')
TextToSpeech = autoclass('android.speech.tts.TextToSpeech')
PythonActivity = autoclass('org.kivy.android.PythonActivity')

#if platform == "android":


Builder.load_string('''
<Text2SpeechDemo>:
    BoxLayout:
        orientation: 'vertical'
        padding: 20

        TextInput:
            id: input_text
            text: 'Put message here'
            
        TextInput:
            id: notification_text
            hint_text: 'errors'
        
        Button:
            text: 'Read'
            size_hint_y: 0.2
            on_press: root.do_read()
''')


class Text2SpeechDemo(BoxLayout):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.tts = TextToSpeech(PythonActivity.mActivity, None)
        # Queue something in french
        self.tts.setLanguage(Locale.FRANCE)

    def do_read(self):
        self.ids.notification_text.text = "reading: %s" % self.ids.input_text.text
        try:
            self.tts.speak(self.ids.input_text.text, TextToSpeech.QUEUE_FLUSH, None)
        except Exception as e:
            self.ids.notification_text.text = str(e)


class MainApp(App):
    def build(self):
        return Text2SpeechDemo()

    def on_pause(self):
        return True


if __name__ == '__main__':
    app = MainApp()
    app.run()