#include <string>
#include <iostream>

using namespace std;

int audio_init(string, string, string);
void audio_listen();
string audio_popCommand();
int audio_getCommandsSize();
int audio_interpetFile(FILE *fh);
void audio_destroy();
string audio_parseCommand(string, string);
