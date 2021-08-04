# 1 "data/vosk/vosk_api.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "data/vosk/vosk_api.h"
# 27 "data/vosk/vosk_api.h"
typedef struct VoskModel VoskModel;




typedef struct VoskSpkModel VoskSpkModel;







typedef struct VoskRecognizer VoskRecognizer;






VoskModel *vosk_model_new(const char *model_path);







void vosk_model_free(VoskModel *model);







int vosk_model_find_word(VoskModel *model, const char *word);






VoskSpkModel *vosk_spk_model_new(const char *model_path);







void vosk_spk_model_free(VoskSpkModel *model);






VoskRecognizer *vosk_recognizer_new(VoskModel *model, float sample_rate);
# 96 "data/vosk/vosk_api.h"
VoskRecognizer *vosk_recognizer_new_spk(VoskModel *model, float sample_rate, VoskSpkModel *spk_model);
# 114 "data/vosk/vosk_api.h"
VoskRecognizer *vosk_recognizer_new_grm(VoskModel *model, float sample_rate, const char *grammar);
# 123 "data/vosk/vosk_api.h"
void vosk_recognizer_set_spk_model(VoskRecognizer *recognizer, VoskSpkModel *spk_model);
# 139 "data/vosk/vosk_api.h"
void vosk_recognizer_set_max_alternatives(VoskRecognizer *recognizer, int max_alternatives);
# 175 "data/vosk/vosk_api.h"
void vosk_recognizer_set_words(VoskRecognizer *recognizer, int words);
# 185 "data/vosk/vosk_api.h"
int vosk_recognizer_accept_waveform(VoskRecognizer *recognizer, const char *data, int length);




int vosk_recognizer_accept_waveform_s(VoskRecognizer *recognizer, const short *data, int length);




int vosk_recognizer_accept_waveform_f(VoskRecognizer *recognizer, const float *data, int length);
# 214 "data/vosk/vosk_api.h"
const char *vosk_recognizer_result(VoskRecognizer *recognizer);
# 228 "data/vosk/vosk_api.h"
const char *vosk_recognizer_partial_result(VoskRecognizer *recognizer);
# 237 "data/vosk/vosk_api.h"
const char *vosk_recognizer_final_result(VoskRecognizer *recognizer);





void vosk_recognizer_reset(VoskRecognizer *recognizer);





void vosk_recognizer_free(VoskRecognizer *recognizer);
# 258 "data/vosk/vosk_api.h"
void vosk_set_log_level(int log_level);






void vosk_gpu_init();






void vosk_gpu_thread_init();
