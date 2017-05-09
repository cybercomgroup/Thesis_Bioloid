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
void demoTurn();
void demodTurn();
bool turnToColor();
int seeColor();
inline void delay(unsigned long ms);

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

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
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;


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
  int ori = -1;
  bool turning = false;

  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return;
  }

  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/face_cascade.xml");
  //SNABBARE OM DET ÄR INTEGER IST FÖR STRING

  Mat img;

  while(1)
  {
    cap >> img;
    ori = image_whereIsCascade(img,cascade,false);
    cout<<ori<<endl;
//    imshow("Image", img);
//    cv::waitKey(5);
    if(ori != -1) //Outside
    {
      if(ori == 4){
        cout<<"Left"<<endl;
      }
      if(ori == 5){
        cout<<"Middle"<<endl;
      }
      if(ori == 6){
        cout<<"Right"<<endl;
      }
    }
    else
    {
      if(!turning){
        turning = true;
        cout<<"Turning left in search detection"<<endl;
      }
      //Timer för
    }
  }
}

void demoTurn()
{
  int ori = -1;
  bool turning = false;

  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return;
  }

  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/face_cascade.xml");

  Mat img;

  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    while(1)
    {
      cap >> img;
      ori = image_whereIsCascade(img,cascade,false);
      cout<<ori<<endl;
      //imshow("Image", img);
      //cv::waitKey(5);
      if(ori != -1) //Outside
      {
        if(ori == 4){
          cout<<"Left"<<endl;
        }
        if(ori == 5){
          cout<<"Middle"<<endl;
        }
        if(ori == 6){
          cout<<"Right"<<endl;
        }
        send_buffer[0] = 'b';
        RS232_SendBuf(comport, send_buffer, 1);
        break;
      }
      else
      {
        if(!turning){
          turning = true;
          cout<<"Turning left in search detection"<<endl;
        }
        //Timer för
      }
    }
    RS232_CloseComport(comport);
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

void demodTurn()
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


  int i = 0;
	int tmp = 0;
  bool turning = false;
 	while( 1 ){
    cap >> frame;
		tmp = image_whereIsCascade(frame,cascade,true);
    imshow("Frame",frame);
    cv::waitKey(5);
    if(tmp == 4)
      send_buffer[0] = 'a';
		else
			send_buffer[0] = 'd';
		RS232_SendBuf(comport, send_buffer, 1);
		send_buffer[0] = 'b';
		RS232_SendBuf(comport, send_buffer, 1);
    if(tmp == 5) //Middle
      return true;
    i++;
		//delay(3000);
	cout<<"I value: " << i << endl;
	}
	cout<<"OUT OF WHILE LOOP"<<endl;
	return false ;
}

int seeColor(){

	return 1;
}

inline void delay(unsigned long ms){
	usleep(ms * 1000);
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
