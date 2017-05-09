
#include "comm/rs232.h"
#include "image/image.h"
#include "audio/audio.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <thread>

using namespace std;
using namespace cv;


#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

void manualMode();
void demoImage();
void demoVoice();
void demoTurn();
bool turnToColor();
int seeColor();
inline void delay(unsigned long ms);

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

bool man = false;
bool rot = false;
int demo = 0;
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
    demoTurn();

  return 0;
}

void demoImage()
{
  string s;
  bool turning = false;
  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/face_cascade.xml");
  //SNABBARE OM DET ÄR INTEGER IST FÖR STRING

  while(1)
  {
    s = "kek";//image_findCascade(cascade,rot);
    if(s.compare("Outside"))
    {
      cout<<s<<endl;
      if(!s.compare("Middle")){
      }
      if(!s.compare("Left")){
      }
      if(!s.compare("Right")){
      }
    }
    else
    {
      if(!turning){
        turning = true;
        cout<<"Turning left in search"<<endl;
      }
      //Timer för
    }
  }
}

void demoVoice()
{
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    string s,c = "";
    bool quit;
    audio_init("audio/lib/2005.lm","audio/lib/2005.dic");
    thread listen(audio_listenForCommand);
    while(!quit)
    {
      if(audio_getCommandsSize() > -1)
      {
        s = "";
        s = audio_popCommand();
        cout<<"Popped command: "<<s<<endl;
        c = "";
        c = audio_parseCommand(s);
        cout<<"Command: "<<c<<endl;
        if(!c.compare("TURN LEFT"))
        {
          send_buffer[0] = 'a';
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
        }
        else if(!c.compare("TURN RIGHT"))
        {
          send_buffer[0] = 'd';
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
        }
        else if(!c.compare("STOP"))
        {
          send_buffer[0] = 'b';
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
        }
        else if(!c.compare("FIND BANANA"))
        {
          send_buffer[0] = 'b';
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
        }
      }
    }
    listen.join();
    //audio_destroy();
  }
}

void testDemo()
{
 send_buffer[0] = 'w';
 RS232_SendBuf(comport, send_buffer, 1);
}

void demoTurn()
{
  char  objectClosetmp;
  bool obC = false;
  int a = 0;
  int b = 0;
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1){
   // while(1){
      //commandAction = commandVoice(); // outcomment for testing
      //testDemo();
      objectClosetmp = RS232_PollComport(comport, send_buffer, 1);

	bool test = turnToColor();

      cout<<test<<endl;
	delay(3000);
/*
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
      }*/
 //   }
    RS232_CloseComport(comport);
  }
}
//Yeah turns to the right color, if there is one
// Finds color return True
// else false
bool turnToColor(){
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/face_cascade.xml");

  Mat frame;

  cap >> frame;

  int i = 0;
	int tmp = image_whereIsCascade(frame,cascade,false,true);
 	while(tmp != 5 && i <9 ){
		if(tmp == 4)
			send_buffer[0] = 'a';
		else
			send_buffer[0] = 'd';
		RS232_SendBuf(comport, send_buffer, 1);
		send_buffer[0] = 'b';
		RS232_SendBuf(comport, send_buffer, 1);
		tmp = image_whereIsCascade(frame,cascade,false,true);
		i++;
		delay(2300);
	cout<<"I value: " << i << endl;
	}
	cout<<"OUT OF WHILE LOOP"<<endl;
	return i == 8 ? true : false ;
}

int seeColor(){

	return 1;
}

inline void delay(unsigned long ms){
	usleep(ms * 1000);
}


void manualMode()
{
  int c = 0;
  cout<<"Test:"<<endl;
  cout<<"1 - Comm"<<endl;
  cout<<"2 - Image"<<endl;
  cin.clear();
  cin>>c;
  switch (c)
  {
    case 1:
    {
      if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
      {
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
      }
      RS232_CloseComport(comport);
      }
    break;
    }
    case 2:
    {
      while(1)
      {
        cv::CascadeClassifier cascade;
        cascade.load("image/cascades/banana_cascade.xml");

        cout<<"Tests:"<<endl;
        cout<<"1 - How many detections"<<endl;
        cout<<"2 - Where is detection 0"<<endl;
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
                cout<<image_getDetections(cascade)<<endl;
                cin.clear();
                cout<<"exit with 0"<<endl;
                cin >> c;
                if(c == 0) {break;}
              }
            break;
            case 2:
              //cout<<image_findCascade(cascade, rot)<<endl;
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
      }
    break;
    }
    default:
      cout<<"Please provide proper command"<<endl;
    break;
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
