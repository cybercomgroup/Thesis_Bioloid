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

void demoImage();
void demoVoice();
bool demoTurn();
void demodTurn();
void mainDemo();
inline void delay(unsigned long ms);
void sysInt();
void testTimeDemo();

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

bool rot = false;
int demo = 1;
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
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;


  if(demo == 1)
//testTimeDemo();   
 demoImage();

  if(demo == 2)
    demoVoice();

  if(demo == 3)
//    mainDemo();

  return 0;
}

void demoImage()
{
  demoTurn();
}
/*
void mainDemo(){
	if(RS232_OpenComport(comport, baudrate, "8N1") != 1){
	//Get voice Command
	//switch(command){}
	


	
	}
	RS232_CloseComport(comport);

}
*/
//void walkToObject()

bool demoTurn()
{
  int ori = -1;
  bool found = false;

  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return false;
  }

  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/face_cascade.xml");

  Mat img;

  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
	    time_t end = time(NULL) + 28;
	    while(time(NULL) <= end && !found)
    {
      cap >> img;
//      cv::flip(img,img,-1); // to flip the camera uncomment this

      ori = image_whereIsCascade(img,cascade,false);
      cout<<ori<<endl;
      imshow("Image", img);
      cv::waitKey(1);
	RS232_SendBuf(comport, send_buffer, 1);
   	
	switch(ori){
		case 4: 
		cout<<"Left"<< endl;
		send_buffer[0] = 'a'; 
		end += 5;
		break;
		
		case 5:
		cout<<"Middle"<<endl;
		send_buffer[0] = 'b';
		found = true;
		break;
		
		case 6:
		cout<<"Right"<<endl;
		send_buffer[0] = 'd';
		end += 5;
		break;
		
		default:
		cout<<"Out of sight"<<endl;
		send_buffer[0] = 'a';
		break;		
	}

        RS232_SendBuf(comport, send_buffer, 1);
//	send_buffer[0] = 'b';
      }
        
      } 
    return found;
    RS232_CloseComport(comport);
  
}
void testTimeDemo(){	
RS232_OpenComport(comport, baudrate, "8N1");
time_t end = time(NULL) +  18; 
	while(time(NULL) < end){
	send_buffer[0] = 'd';
	RS232_SendBuf(comport, send_buffer, 1);
	send_buffer[0] = 'b';
	RS232_SendBuf(comport, send_buffer, 1);
	
}	

//	time_t end = time(NULL) +5;
//	while(time(NULL) < end)
//	 cout<<"time: "<< time(NULL)<< endl;
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

//Opens the porst and stuff
void sysInt(){
  RS232_OpenComport(comport, baudrate, "8N1");
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
