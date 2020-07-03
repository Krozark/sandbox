from pydub import AudioSegment
import logging
import subprocess
import signal
from tempfile import NamedTemporaryFile
from pydub.utils import get_player_name
import time
from enum import Enum


class Sound(object):
    FFPLAY_PLAYER = get_player_name()

    class STATUS(Enum):
        STOPPED = 1
        PLAYING = 2
        PAUSED = 3
        ERROR = 10

    def __init__(self, segment):
        self._segment = segment
        self._tmp_file = None
        self._popen = None
        self._status = self.STATUS.STOPPED

    def __del__(self):
        if self._popen:
            self._popen.kill()
            self._popen = None

        if self._tmp_file:
            self._tmp_file.close()

    def _create_tmp(self):
        self._tmp_file = NamedTemporaryFile("w+b", suffix=".wav")
        self._segment.export(self._tmp_file.name, "wav")

    def _create_popen(self):
        self._popen = subprocess.Popen([self.FFPLAY_PLAYER, "-nodisp", "-autoexit", "-hide_banner", self._tmp_file.name])

    def play(self):
        if self._status not in (self.STATUS.STOPPED, self.STATUS.PAUSED):
            raise Exception()

        if self._tmp_file is None:
            self._create_tmp()

        if self._popen is None:
            self._create_popen()
        elif self._status == self.STATUS.PAUSED:
            self._popen.send_signal(signal.SIGCONT)

        self._status = self.STATUS.PLAYING

    def wait(self, timeout=None):
        code = self._popen.wait(timeout=timeout)
        return code

    def poll(self):
        code = self._popen.poll()
        return code

    def pause(self):
        if self._status != self.STATUS.PLAYING:
            raise Exception()

        self._popen.send_signal(signal.SIGSTOP)
        self._status = self.STATUS.PAUSED

    def stop(self):
        if self._status != self.STATUS.PLAYING:
            raise Exception()

        if self._popen:
            self._popen.kill()
            self._popen = None

        if self._tmp_file:
            self._tmp_file.close()

        self._status = self.STATUS.STOPPED

l = logging.getLogger("pydub.converter")
l.setLevel(logging.DEBUG)
l.addHandler(logging.StreamHandler())


song = AudioSegment.from_ogg("music.ogg")

sound = Sound(song)
sound2 = Sound(song)

sound.play()
time.sleep(5)

sound2.play()
sound.pause()
time.sleep(5)

sound.play()
time.sleep(5)

sound.stop()
time.sleep(5)
sound2.wait()







