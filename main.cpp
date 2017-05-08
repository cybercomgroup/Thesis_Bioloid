
#include "comm/rs232.h"
#include "image/image.h"
#include "audio/audio.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;
using namespace cv;


#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

void manualMode();
void demoImage();
void demoVoice();
void mainMode();

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

bool man = false;
bool rot = false;
int demo = 3;
int comport = 16; //pNUMBER
int baudrate = 57600; //bNUMBER

//REMOVE THESE LATER
unsigned char receive_buffer[RECEIVE_CHARS];
unsigned char send_byte = 42;
unsigned char send_buffer[SEND_CHARS];

/*
Usage:
To change running parameters use add the arguments described above when running the program.
Example: ./main p123 sets the comport to 123
*/
int main (int argc, char *argv[]) {
  //Set up:
  if(argc > 1)
  {
    if(!cParser((argc),argv))
    return 0;
  }

  cout<<"The program is running these values:"<<endl;
  cout<<"Demo: "<<demo<<endl;
  cout<<"Manualmode: "<<man<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;

  if(man)
  manualMode();

  if(demo == 1)
    demoImage();

  if(demo == 2)
    demoVoice();

  if(demo == 3)
    mainMode();

  return 0;
}

void demoImage()
{
  string s = "";
  bool ready = true;
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    cv::CascadeClassifier cascade;
    cascade.load("image/cascades/face_cascade.xml");
    //SNABBARE OM DET ÄR INTEGER IST FÖR STRING
    while(s.compare("Middle"))
    {
      s = image_findCascade(cascade, rot);
      if(ready)
      {
        ready = false;
        if(!s.compare("Left") || s.compare("Outside"))
        {
          //SEND TURN LEFT
        }
        if(!s.compare("Right"))
        {
          //SEND TURN right
        }
      }
      //Ready on timer not to overflow commands
    }
    //SEND POINT
    RS232_CloseComport(comport);
  }
}

void demoVoice()
{
  string s = "";
  bool quit;
  audio_init("audio/lib/5993.lm","audio/lib/5993.dic");
  audio_listenForCommand();
  while(!quit)
  {
    if(audio_getCommandsSize() > -1)
    {
      s = audio_popCommand();
      cout<<"You said: "<< s <<endl;
      s = audio_parseCommand(s);
      if(!s.compare("TURN LEFT"))
      {
        send_buffer[0] = 'a';
        return;
      }
      else if(!s.compare("TURN RIGHT"))
      {
        send_buffer[0] = 'd';
        return;
      }
      else if(!s.compare("GO")) //TEMP
      {
        send_buffer[0] = 'b';
        return;
      }
    }
  }
  //audio_destroy();
}

void testDemo()
{
  send_buffer[0] = 'w';
}

void mainMode()
{
  char  objectClosetmp;
  bool obC = false;
  int a = 0;
  int b = 0;
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1){
    while(1){
      //commandAction = commandVoice(); // outcomment for testing
      testDemo();
      objectClosetmp = RS232_PollComport(comport, send_buffer, 1);
      cout<<objectClosetmp<<endl;
      if(objectClosetmp == 'g'){
        a++;
        cout<< "A higher" << endl;
      }else if(objectClosetmp == 'b'){
        b++;
        cout<< "B higher"<< endl;
      }
      if(a > 50){
        obC = false;
        b = a = 0;
      }else if( b > 50){
        obC = true;
        b = a = 0;
      }
      //cout<<objectClose<<endl;
      if(send_buffer[0] != '0' && !obC){
        cout<<"not close"<<endl;
        //       RS232_SendBuf(comport, send_buffer, 1);
      }else{
        send_buffer[0] = 'd';
        cout<< "close"<<endl;
        //RS232_SendBuf(comport, send_buffer, 1);
      }
    }
    RS232_CloseComport(comport);
  }
}

void manualMode()
{
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    int c;
    while(1)
    {
      cout<<"Tests:"<<endl;
      cout<<"1 - Write buffer"<<endl;
      cout<<"2 - Write byte"<<endl;
      cout<<"0 - Exit"<<endl;
      c = 0;
      cin.clear();
      cin>>c;
      if(!cin.fail())
      {
        switch(c)
        {
          case 0:
          return;
          break;
          case 1:
          while(1)
          {
            cout<<"Write buffer to send:"<<endl;
            cin.clear();
            cin >> send_buffer;
            if(send_buffer[0] == '0') {break;}
            RS232_SendBuf(comport, send_buffer, SEND_CHARS);
          }
          break;
          case 2:
          cout<<"Not implemented"<<endl;
          /*
          while(1)
          {
          cout<<"Write buffer to send:"<<endl;
          cin.clear();
          cin >> send_byte;
          if(send_byte == '0') {break;}
          RS232_SendBuf(comport, send_byte);
        }*/
        break;
        default:
        cout<<"Please provide proper input"<<endl;
        break;
      }
    }
    else
    {
      cout<<"Please provide proper input"<<endl;
    }
    /*
    cout<<"Respone:"<<endl;
    RS232_PollComport(comport, receive_buffer, RECEIVE_CHARS);
    cout<<receive_buffer<<endl; */
  }
  RS232_CloseComport(comport);
}
}

bool cParser(int argN, char *argv[])
{
  int number;
  string input;
  for(int i = 1; i < argN; i++)
  {
    input = argv[i];
    if(input.length() > 1)
    {
      number = stoi(string(argv[i]).substr(1).c_str(),nullptr);
    }
    switch(argv[i][0])
    {
      case 'm':
      man = true;
      break;

      case 'r':
      rot = true;
      break;

      case 'd':
      demo = number;
      break;

      case 'p':
      comport = number;
      break;

      case 'b':
      baudrate = number;
      break;

      case 'h':
      cout<<"m for manual"<<endl;
      cout<<"r to rotate camera image"<<endl;
      cout<<"d<NUMBER> for demo"<<endl;
      cout<<"p<NUMBER> for input of comport"<<endl;
      cout<<"b<NUMBER> for input of baudrate"<<endl;
      return false;
      break;

      default:
      break;
    }
    return true;
  }
}
