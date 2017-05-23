#include "comm/rs232.h"
#include "image/image.h"
#include "audio/audio.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <unistd.h> 

using namespace std;
using namespace cv;


#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

void demoImage();
void demoVoice();
bool demoTurn();
//void demodTurn();
void realDemoa();
inline void delay(unsigned long ms);
void testTimeDemo();
void findThing();
void sendStop();

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);

int cameraDevice = 0;
string cascadeFile = "face_cascade.xml";
bool rotImage = false;
bool cameraFeed = false;
bool printDetectionTime = false;
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
  if(!cParser(argc, argv))
    return 0;


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
 realDemoa(); 
// demoTurn();
}

void realDemoa(){
	bool object = false;
	if(RS232_OpenComport(comport, baudrate, "8N1") != 1){
	//Get voice Command
	//switch(command){}

	object = demoTurn();
//	delay(1000);
	usleep(750000);
	send_buffer[0] = 'b';
	RS232_SendBuf(comport, send_buffer, SEND_CHARS);
	cout<<object<<" Did We get here "<< endl;	
	send_buffer[0] = 'w';
	RS232_SendBuf(comport, send_buffer, SEND_CHARS);
	RS232_PollComport(comport, receive_buffer, 1);
	char tmp = receive_buffer[0]; 
	cout<<tmp<<endl; 
	while(tmp == 'g'){
		send_buffer[0]= 'w';
		RS232_SendBuf(comport, send_buffer,SEND_CHARS);
		 RS232_PollComport(comport, receive_buffer, 1);
		tmp = receive_buffer[0];
		cout<<tmp<<endl; 
	}
//	delay(5000); 
	usleep(10000);
	send_buffer[0] = 'b';
	RS232_SendBuf(comport, send_buffer, SEND_CHARS); 
	usleep(15000);
	send_buffer[0] = 'u';
	RS232_SendBuf(comport, send_buffer, SEND_CHARS);

	usleep(25000);	
	if(object == true){
		send_buffer[0] = 'c';
		RS232_SendBuf(comport, send_buffer, SEND_CHARS);
}
	else
		findThing();
		
	//RS232_SendBuf(comport, send_buffer, SEND_CHARS);
	}
	RS232_CloseComport(comport);

}

void findThing(){
 int tmp = rand() % 2;

	if(tmp == 0)
		send_buffer[0] = 'a';
	else 
	  	send_buffer[0] = 'd';

	RS232_SendBuf(comport, send_buffer, SEND_CHARS); 
	usleep(1000000);
	sendStop();
	
	
}

void sendStop(){
	send_buffer[0] = 'b';
	RS232_SendBuf(comport, send_buffer, SEND_CHARS);	
}

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

 // if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  //{
    time_t end = time(NULL) + 18;
    while(time(NULL) <= end && !found)
    {
      cap >> img;
      cv::flip(img,img,-1); // to flip the camera uncomment this

      ori = image_whereIsCascade(img,cascade,false);
      cout<<"Time right Now: "<<time(NULL)<< " Time we aim for: "<< end <<endl;
      //imshow("Image", img);
      cv::waitKey(1);
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

     // }
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
    audio_init("audio/lib/en-us","audio/lib/2005.lm","audio/lib/2005.dic");
    thread listen(audio_listen);
    while(!quit)
    {
      if(audio_getCommandsSize() > -1)
      {
        s = "";
        s = audio_popCommand();
        cout<<"Popped command: "<<s<<endl;
        c = "";
        c = audio_parseCommand("ROBOT",s);
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
inline void delay(unsigned long ms){
	usleep(ms * 1000); 
}


bool cParser( int argc, char** argv )
{
  int opt;
  while ((opt = getopt (argc, argv, "c:sfpd:h")) != -1)
  {
    switch (opt)
    {
      case 'c':
        cameraDevice = atoi(optarg);
        break;
      case 's':
        cameraFeed = !cameraFeed;
        break;
      case 'f':
        rotImage = !rotImage;
        break;
      case 'p':
        printDetectionTime = !printDetectionTime;
        break;
      case 'd':
        demo = atoi(optarg);
        break;
      case 'x':
        cascadeFile = optarg;
        break;
      case 'h':
        cout<<"-c [arg] capture device"<<endl;
        cout<<"-x [arg] cascade file inside /cascades"<<endl;
        cout<<"-f rotates the camera feed"<<endl;
        cout<<"-s show camera feed to user"<<endl;
        cout<<"-p turn on/off detection time prints"<<endl;
        cout<<"-d [arg] set what demo to use"<<endl;

        cout<<"Available demos:"<<endl;
        cout<<"1 - detectionTest"<<endl;
        cout<<"2 - orientationTest"<<endl;
        cout<<"3 - numDetectionsTest"<<endl;
        cout<<endl;
        return false;
        break;
    }
  }
  cout<<"The program is run using these settings:"<<endl;
  cout<<"Capture device: "<<cameraDevice<<endl;
  cout<<"Cascade file: "<<cascadeFile<<endl;
  cout<<"Rotation: "<<rotImage<<endl;
  cout<<"Camera feed: "<<cameraFeed<<endl;
  cout<<"Print cascade detection time: "<<printDetectionTime<<endl;
  cout<<"Demo: "<<demo<<endl;
  return true;
}
