from kivy.lang import Builder
from plyer import gps
from kivy.app import App
from kivy.properties import StringProperty
from kivy.clock import mainthread
from kivy.utils import platform
from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

import logging

logger = logging.getLogger(__name__)

logging.basicConfig(level=logging.DEBUG)

Builder.load_string('''
<GPSDemo>:
    BoxLayout:
        orientation: 'vertical'
        
        Label:
            text: root.gps_location
        
        Label:
            text: root.gps_status
        
        Button:
            id: toggle_button
            text: 'start'
            size_hint_y: 0.2
            on_press: root.do_toggle()
        
''')

class GPSDemo(BoxLayout):
    gps_location = StringProperty()
    gps_status = StringProperty('Click Start to get GPS location updates')

    def __init__(self, parent, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._parent_app = parent

    def do_toggle(self):
        current_status = self.ids.toggle_button.text
        if current_status.lower() == "start":
            self.do_start()
            self.ids.toggle_button.text = "stop"
        else:
            self.do_stop()
            self.ids.toggle_button.text = "start"

    def do_start(self):
        self.gps_location = ""
        self.gps_status = ""
        self._parent_app.start()

    def do_stop(self):
        self.gps_location = ""
        self.gps_status = ""
        self._parent_app.stop()

    def set_gps_status(self, stype, status):
        self.gps_status = 'type={}\n{}'.format(stype, status)

    def set_gps_location(self, **kwargs):
        self.gps_location = "\n".join("{} : {}".format(k, v) for k, v in kwargs.items())


class MainApp(App):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


    def request_android_permissions(self):
        """
        Since API 23, Android requires permission to be requested at runtime.
        This function requests permission and handles the response via a
        callback.

        The request will produce a popup if permissions have not already been
        been granted, otherwise it will do nothing.
        """
        from android.permissions import request_permissions, Permission

        def callback(permissions, results):
            """
            Defines the callback to be fired when runtime permission
            has been granted or denied. This is not strictly required,
            but added for the sake of completeness.
            """
            if all([res for res in results]):
                print("callback. All permissions granted.")
            else:
                print("callback. Some permissions refused.")

        request_permissions(
            [
                Permission.ACCESS_COARSE_LOCATION,
                Permission.ACCESS_FINE_LOCATION
            ],
            callback
        )

    def build(self):
        self.gps_ui = GPSDemo(self)
        try:
            gps.configure(
                on_location=self.on_location,
                on_status=self.on_status
            )
        except NotImplementedError:
            import traceback
            traceback.print_exc()
            self.gps_ui.gps_status = 'GPS is not implemented for your platform'

        if platform == "android":
            print("gps.py: Android detected. Requesting permissions")
            self.request_android_permissions()
        return self.gps_ui

    def start(self, minTime=500, minDistance=0):
        gps.start(minTime, minDistance)

    def stop(self):
        gps.stop()

    @mainthread
    def on_location(self, **kwargs):
        self.gps_ui.set_gps_location(**kwargs)

    @mainthread
    def on_status(self, stype, status):
        self.gps_ui.set_gps_status(stype, status)

    def on_pause(self):
        gps.stop()
        return True

    def on_resume(self):
        self.start()


if __name__ == '__main__':
    MainApp().run()
