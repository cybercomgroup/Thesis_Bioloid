#include "audio.h"
#include <thread>
using namespace std;

string parseCommand(string s);

int main(int argc, char *argv[])
{
  string s;
  int pos;
  bool printM = true;
  audio_init("lib/2005.lm","lib/2005.dic");
  thread listen(audio_listenForCommand);
  while(1)
  {
    if(printM)
    {
      cout<<"Listening.."<<endl;
      printM = false;
    }
    if(audio_getCommandsSize() > 0){
      printM = true;
      cout<<"Heard this:"<<endl;

      s="";
      s=audio_popCommand();
      cout<<"Popped command: "<<s<<endl;
      cout<<"Command: "<<audio_parseCommand(s)<<endl;
    }
  }
  listen.join();
  return 0;
}
