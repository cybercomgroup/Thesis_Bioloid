#include "audio.h"
#include "voce.h"


void audio_init(const std::string &vocePath, bool initSynthesis, bool initRecognition, const std::string &grammarPath, const std::string &grammarName)
{
	voce::init(vocePath, initSynthesis, initRecognition, grammarPath, grammarName);
}

void audio_destroy()
{
	voce::destroy();
}

void audio_synthesize(const std::string &message)
{
	voce::synthesize(message);
}

void audio_stopSynthesizing()
{
	voce::stopSynthesizing();
}

int audio_getRecognizerQueueSize()
{
	return voce::getRecognizerQueueSize();
}

std::string audio_popRecognizedString ()
{
	voce::popRecognizedString();
}

void audio_setRecognizerEnabled (bool e)
{
	voce::setRecognizerEnabled(e);
}

bool audio_isRecognizerEnabled ()
{
	return voce::isRecognizerEnabled();
}
