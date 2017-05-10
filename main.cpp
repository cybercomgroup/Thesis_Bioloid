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
void mainDemo();
bool turnToColor();
int seeColor();
inline void delay(unsigned long ms);

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

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
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;


  if(demo == 1)
    demoImage();

  if(demo == 2)
    demoVoice();

  if(demo == 3)
    mainDemo();

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

void mainDemo(){
  sysInt();
	while(1){

		//voice command
		//l�gg in n�got har som g�r att man f�r ett int v�rde som best�mmer vad vi g�r.
	//	switch(command){
	//	}
	int i = 9;
	bool turnTo = turnToColor();
	while(!turnTo && i > 0){
		cout<< "Did we get here" << endl;
		turnTo = turnToColor();
//		i--;
	//		delay(2000);
	}
	cout<<"did we get here"<<endl;
	while( i < 50 )
	{
	send_buffer[0] = 'w';
	RS232_SendBuf(comport, send_buffer, 1);
	cout<<"in the while"<<endl;
	object = RS232_PollComport(comport, receive_buffer, 1);
	cout<<receive_buffer<<endl;
	if(receive_buffer[0]  == 'b') i++;
	else i--;
	}
	send_buffer[0] = 'b';
		RS232_SendBuf(comport, send_buffer, 1);
  }
	RS232_CloseComport(comport);

}

//void walkToObject()

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
	  send_buffer[0] = 'a';
       	  RS232_SendBuf(comport, send_buffer, 1);
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

//Opens the porst and stuff
void sysInt(){
  RS232_OpenComport(comport, baudrate, "8N1")

  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  cv::CascadeClassifier cascade;
  cascade.load("image/cascades/controller_cascade.xml");

  Mat frame;

  cap >> frame;
  imshow("Frame",frame);
  cv::waitKey(5);

}

void testDemo()
{
 send_buffer[0] = 'w';
 RS232_SendBuf(comport, send_buffer, 1);
}

//Yeah turns to the right color, if there is one
// Finds color return True
// else false
bool turnToColor(){



  int i = 0;
	int tmp = 0;
  bool turning = false;
 	//while( 1 ){
  tmp = image_whereIsCascade(frame,cascade,true);
	cout<<"Value is: " << tmp<< endl;
	switch(tmp){
	case 4:
		send_buffer[0] = 'a'; break;
	case 5:
		return true; break;
	case 6:
		send_buffer[0] = 'd'; break;
	default:
		send_buffer[0] = 'd'; break;

	}

	RS232_SendBuf(comport, send_buffer, 1);
	send_buffer[0] = 'b';
	RS232_SendBuf(comport, send_buffer, 1);

	//}
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
