#include <string>
#include <iostream>

void audio_init(const std::string &vocePath, bool initSynthesis, bool initRecognition, const std::string &grammarPath, const std::string &grammarName);

void audio_destroy();

void audio_synthesize(const std::string &message);

void audio_stopSynthesizing();

int audio_getRecognizerQueueSize();

std::string audio_popRecognizedString ();

void audio_setRecognizerEnabled (bool e);

bool audio_isRecognizerEnabled ();
