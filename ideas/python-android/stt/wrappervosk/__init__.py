from plyer.utils import platform


if platform == "android":
    from .android import (
        Model,
        SpkModel,
        KaldiRecognizer,
        SetLogLevel,
        #GpuThreadInit
    )
else:
    from vosk import (
        Model,
        SpkModel,
        KaldiRecognizer,
        SetLogLevel
    )

