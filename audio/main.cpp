#include "audio.h"
#include <thread>
using namespace std;

string parseCommand(string s);

int main(int argc, char *argv[])
{
  string s;
  int pos;
  audio_init("lib/5993.lm","lib/5993.dic");
  thread listen(audio_listenForCommand);
  while(1)
  {
    if(audio_getCommandsSize() > -1){
      s="";
      s=audio_popCommand();
      cout<<"Popped command: "<<s<<endl;
      cout<<"Command: "<<audio_parseCommand(s)<<endl;
    }
  }
  listen.join();
  return 0;
}
