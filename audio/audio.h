#include <string>
#include <iostream>

class audio {
public:
  void audio_init(const std::string &vocePath="audio/lib", bool initSynthesis=false, bool initRecognition=true, const std::string &grammarPath="audio/grammar", const std::string &grammarName="control");

  void audio_destroy();

  void audio_synthesize(const std::string &message);

  void audio_stopSynthesizing();

  int audio_getRecognizerQueueSize();

  std::string audio_popRecognizedString ();

  void audio_setRecognizerEnabled (bool e);

  bool audio_isRecognizerEnabled ();
};
