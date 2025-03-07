def with_whisper():
    import whisper

    model = whisper.load_model("base")

    # load audio and pad/trim it to fit 30 seconds
    audio = whisper.load_audio("audio.mp3")
    audio = whisper.pad_or_trim(audio)

    # make log-Mel spectrogram and move to the same device as the model
    mel = whisper.log_mel_spectrogram(audio).to(model.device)

    # detect the spoken language
    _, probs = model.detect_language(mel)
    print(f"Detected language: {max(probs, key=probs.get)}")

    # decode the audio
    options = whisper.DecodingOptions()
    result = whisper.decode(model, mel, options)

    # print the recognized text
    print(result.text)


def with_faster_whisper():
    # https://developer.nvidia.com/cudnn
    # sudo apt-get install -y libcudnn8
    # https://developer.nvidia.com/hpc-sdk-downloads

    import faster_whisper  # faster-whisper==1.0.3
    import torch  # torch==2.3.1

    device = "cuda" if torch.cuda.is_available() else "cpu"

    model = faster_whisper.WhisperModel(
        model_size_or_path="base",
        device=device,
        compute_type="float16",
        device_index=0
    )

    segments, info = model.transcribe("audio.mp3", beam_size=5)

    print("Detected language '%s' with probability %f" % (info.language, info.language_probability))

    for segment in segments:
        print("[%.2fs -> %.2fs] %s" % (segment.start, segment.end, segment.text))


import pyaudio

AUDIO_RATE = 16000
AUDIO_CHUNK = int(AUDIO_RATE * 40 / 1000)
AUDIO_CHANNELS = 1
AUDIO_FORMAT = pyaudio.paInt16
AUDIO_SIZE = pyaudio.get_sample_size(AUDIO_FORMAT)
INT16_MAX_ABS_VALUE = 32768.0
AUDIO_MAX_QUEUE_SIZE = int(AUDIO_RATE / AUDIO_CHUNK * 4)  # 4 secondes of recording
AUDIO_SAME_MATCH_NUMBER = 3
AUDIO_MIN_PREFIX_SIZE = 5


def _find_tail_match_in_text(text1, text2, length_of_match=10):
    """
    Find the position where the last 'n' characters of text1
    match with a substring in text2.

    This method takes two texts, extracts the last 'n' characters from
    text1 (where 'n' is determined by the variable 'length_of_match'), and
    searches for an occurrence of this substring in text2, starting from
    the end of text2 and moving towards the beginning.

    Parameters:
    - text1 (str): The text containing the substring that we want to find
      in text2.
    - text2 (str): The text in which we want to find the matching
      substring.
    - length_of_match(int): The length of the matching string that we are
      looking for

    Returns:
    int: The position (0-based index) in text2 where the matching
      substring starts. If no match is found or either of the texts is
      too short, returns -1.
    """

    # Check if either of the texts is too short
    if len(text1) < length_of_match or len(text2) < length_of_match:
        return -1

    # The end portion of the first text that we want to compare
    target_substring = text1[-length_of_match:]

    # Loop through text2 from right to left
    for i in range(len(text2) - length_of_match + 1):
        # Extract the substring from text2
        # to compare with the target_substring
        current_substring = text2[len(text2) - i - length_of_match:
                                  len(text2) - i]

        # Compare the current_substring with the target_substring
        if current_substring == target_substring:
            # Position in text2 where the match starts
            return len(text2) - i

    return -1


def with_faster_whisper_and_microphone():
    # https://developer.nvidia.com/cudnn
    # sudo apt-get install -y libcudnn8
    # https://developer.nvidia.com/hpc-sdk-downloads

    import faster_whisper  # faster-whisper==1.0.3
    import torch  # torch==2.3.1
    from queue import Queue
    import threading
    import numpy as np
    import os
    import time

    device = "cuda" if torch.cuda.is_available() else "cpu"

    model = faster_whisper.WhisperModel(
        model_size_or_path="base",
        device=device,
        compute_type="float16",
        # device_index=0
    )

    audio_queue = Queue()
    event = threading.Event()
    audio_frames = []
    text_storage = []
    stabilized_text = ""
    new_sentence = True
    thread = threading.Thread(target=microphone, args=(audio_queue, event))
    thread.deamon = True
    thread.start()

    while not event.is_set():
        try:
            print("---------------")
            # avoid usless CPU computation
            time.sleep(0.2)

            # consume all possible audio chunks
            while audio_queue.qsize() > 0:
                data = audio_queue.get()
                audio_frames.append(data)

            # Convert the buffer frames to a NumPy array
            audio_array = np.frombuffer(
                b''.join(audio_frames),
                dtype=np.int16
            )
            # Normalize the array to a [-1, 1] range
            audio_array = audio_array.astype(np.float32) / INT16_MAX_ABS_VALUE

            # Perform transcription and assemble the text
            segments, info = model.transcribe(
                audio_array,
                language="fr",
                beam_size=5,
                vad_filter=True,  # remove blanks
                hallucination_silence_threshold=1,
            )

            transcription = " ".join(seg.text for seg in segments)
            transcription = transcription.strip()
            text_storage.append(transcription)
            new_sentence = new_sentence or not transcription

            print("Transcription: ", transcription)
            print("stabilized_text: ", stabilized_text)
            print("new_sentence: ", new_sentence)

            # Take the last x texts in storage, if they exist
            if len(text_storage) >= AUDIO_SAME_MATCH_NUMBER:
                # Find the longest common prefix between the two texts
                common_prefix = os.path.commonprefix(text_storage[-AUDIO_SAME_MATCH_NUMBER:])

                print("common_prefix: ", common_prefix)

                common_prefix = common_prefix.rstrip("...").strip()

                if len(common_prefix) <= AUDIO_MIN_PREFIX_SIZE:
                    continue

                # def f1():
                #     stabilized_text_part = stabilized_text[-len(common_prefix):]
                #
                #     print("stabilized_text_part: ", stabilized_text_part)
                #
                #     # now we need to add the new translated text to stabilized_text
                #     match = SequenceMatcher(None, stabilized_text_part, common_prefix).find_longest_match(0,
                #                                                                                           len(stabilized_text_part),
                #                                                                                           0,
                #                                                                                           len(common_prefix))
                #     print("match stabilized_text:", stabilized_text_part[match.a:match.a + match.size])
                #     print("match common_prefix:", common_prefix[match.b:match.b + match.size])
                #     print("adding:", common_prefix[match.b + match.size:])
                #
                #     stabilized_text += common_prefix[match.b + match.size:]

                if new_sentence:
                    stabilized_text += common_prefix
                    new_sentence = False
                else:
                    find = _find_tail_match_in_text(stabilized_text, common_prefix,
                                                    length_of_match=max(AUDIO_MIN_PREFIX_SIZE,
                                                                        min(10, len(stabilized_text))))
                    if find >= 0:
                        stabilized_text += common_prefix[find:]
                        new_sentence = False

                print("stabilized_text: ", stabilized_text)
            audio_frames = audio_frames[-AUDIO_MAX_QUEUE_SIZE:]
        except KeyboardInterrupt:
            event.set()
            break


def microphone(audio_queue, shutdown_event):
    _pyaudio = pyaudio.PyAudio()

    stream = _pyaudio.open(
        rate=AUDIO_RATE,
        format=AUDIO_FORMAT,
        channels=AUDIO_CHANNELS,
        input=True,
        frames_per_buffer=AUDIO_CHUNK
    )

    try:
        while not shutdown_event.is_set():
            try:
                data = stream.read(AUDIO_CHUNK)
                if len(data):
                    audio_queue.put(data)
                    # print("Putting %s data in queue" % len(data))
            except Exception as e:
                print(e)

    finally:
        stream.stop_stream()
        stream.close()
        _pyaudio.terminate()


if __name__ == "__main__":
    with_faster_whisper()
    with_faster_whisper_and_microphone()
