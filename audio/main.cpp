#include "audio.h"
#include <thread>
using namespace std;

string parseCommand(string s);

int main(int argc, char *argv[])
{
  string s;
  int pos;
  audio_init("./5993.lm","./5993.dic");
  thread listen(audio_listenForCommand);
  while(1)
  {
    if(audio_getCommandsSize() > -1){
      s="";
      s=audio_popCommand();
      cout<<"Popped command: "<<s<<endl;
      cout<<"Command: "<<parseCommand(s)<<endl;
    }
  }
  listen.join();
  return 0;
}

string parseCommand(string s)
{
  //string command;
  int pos = s.find_first_of(" \t");
  if(pos != -1)
  {
    if(s.substr(0,pos).compare("ROBOT") == 0)
    {
      return s.substr(pos);
    }
  }
}
