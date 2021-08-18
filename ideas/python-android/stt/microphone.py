import logging

from jnius import autoclass

logger = logging.getLogger(__name__)

MediaRecorder = autoclass('android.media.MediaRecorder')
AudioSource = autoclass('android.media.MediaRecorder$AudioSource')
AudioRecord = autoclass('android.media.AudioRecord')
AudioRecordBuilder = autoclass('android.media.AudioRecord$Builder')
AudioFormat = autoclass('android.media.AudioFormat')
AudioFormatBuilder = autoclass('android.media.AudioFormat$Builder')
OutputFormat = autoclass('android.media.MediaRecorder$OutputFormat')
AudioEncoder = autoclass('android.media.MediaRecorder$AudioEncoder')

AUDIO_CHUNK = 8000


class _AndroidAudioStream(object):
    def __init__(self, rec):
        self._rec = rec
        self._array = bytearray(AUDIO_CHUNK)

    def read(self, chunk_size):
        logger.debug("Reading %s bytes", chunk_size)
        chunk_size = min(chunk_size, AUDIO_CHUNK)
        nb_bytes = self._rec.read(self._array, 0, chunk_size)
        logger.debug("Read %s bytes", nb_bytes)
        res = None
        if nb_bytes >= 0:
            res = self._array[:nb_bytes]
        else:
            # error case
            logger.warning("Error '%s' while reading data.", nb_bytes)

        return res


class AndroidMicrophoneEngine(object):
    """
    Class that run a task in background, on put to it's output tha audio listen
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._rec = None

    def start(self):
        #AUDIO_RATE = 16000
        AUDIO_RATE = 44100
        min_buff_size = AudioRecord.getMinBufferSize(AUDIO_RATE, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT) # AUDIO_CHUNK
        print("min buf size: %s" % min_buff_size)
        # self._rec = AudioRecordBuilder()\
        #     .setAudioSource(AudioSource.DEFAULT)\
        #     .setAudioFormat(
        #         AudioFormatBuilder()
        #         .setEncoding(AudioFormat.ENCODING_PCM_16BIT)
        #         #.setSampleRate(AUDIO_RATE)
        #         .setChannelMask(AudioFormat.CHANNEL_IN_MONO)
        #         .build()
        #     ).setBufferSizeInBytes(2*min_buff_size).build()
        # self._rec = AudioRecord(
        #     AudioSource.MIC,
        #     AUDIO_RATE,
        #     AudioFormat.CHANNEL_IN_MONO, # guaranted to work on every devices
        #     AudioFormat.ENCODING_PCM_16BIT,
        #     AudioRecord.getMinBufferSize(AUDIO_RATE, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT) # AUDIO_CHUNK
        # )
        # self._rec = AudioRecord(
        #     AudioSource.VOICE_RECOGNITION,
        #     AUDIO_RATE,
        #     AudioFormat.CHANNEL_IN_MONO, # guaranted to work on every devices
        #     AudioFormat.ENCODING_PCM_16BIT,
        #     AUDIO_RATE * 2
        # )
        # self._rec.startRecording()



    def stop(self):
        if self._rec:
            self._rec.stop()
            self._rec.release()
            self._rec = None

    def create_stream(self):
        return _AndroidAudioStream(self._rec)


def instance_class():
    return AndroidMicrophoneEngine
