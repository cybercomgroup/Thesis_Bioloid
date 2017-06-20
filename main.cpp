#include "comm/rs232.h"
#include "image/image.h"
#include "audio/audio.h"
#include "tools/INIReader.h"

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

void demoVoice();
void demo();
void findObject(string cascade);
bool turnTo(string cascade);
void avoidObstacle();
void sendStop();
void walkForward();

//Parser and global belonging variables
bool cParser(int argN, char *argv[]);
void printSettings();
bool initConf(string);
void initAll(string);

int cameraDevice = 0;
string configFile = "tools/config-rasp.ini";
string cascadeFile = "face_cascade.xml";
bool rotImage = false;
bool cameraFeed = false;
bool printDetectionTime = false;
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
  if(!cParser(argc,argv))
  return 0;
  initAll(configFile);
  printSettings();
  audio_printConf();

  demo();
  return 0;
}
void demo()
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
        else if(!c.compare("FIND FACE"))
        {
          findObject("image/cascades/face_cascade.xml");
        }
        else if(!c.compare("FIND CONTROLLER"))
        {
          findObject("image/cascades/controller_cascade.xml");
        }
        else if(!c.compare("FIND BANANA"))
        {
          findObject("image/cascades/banana_cascade.xml");
        }
      }
    }
    listen.join();
    //audio_destroy();
  }

}



void findObject(string cascadef){
  bool object = false;
  do{
    object = turnTo(cascadef);
    sleep(3);

    walkForward();

    send_buffer[0] = 'u';
    RS232_SendBuf(comport, send_buffer, SEND_CHARS);
    sleep(3);

    if(object == true){
      //send_buffer[0] = 'c';
      //RS232_SendBuf(comport, send_buffer, 1);
      break;
    }

    avoidObstacle();



  }while(!object);

}


void avoidObstacle(){
  send_buffer[0] = 's';
  RS232_SendBuf(comport, send_buffer, 1);
  sleep(2);
  send_buffer[0] = 'b';
  RS232_SendBuf(comport, send_buffer, 1);
  sleep(2);
  int tmp = rand() % 2;
  cout<<"plz"<<endl;
  if(tmp == 0)
  send_buffer[0] = 'a';
  else
  send_buffer[0] = 'd';

  RS232_SendBuf(comport, send_buffer, 1);

  sleep(4);
  send_buffer[0] = 'b';
  RS232_SendBuf(comport, send_buffer, 1);


}

void walkForward()
{
  unsigned char receive1[RECEIVE_CHARS];

  send_buffer[0] = 'w';
  RS232_SendBuf(comport, send_buffer, 1);

  char tmp;
  while(tmp != 'b'){
    RS232_PollComport(comport, receive1, 1);
    tmp = receive1[0];
    cout<<tmp<<endl;
  }
  send_buffer[0] = 'b';
  RS232_SendBuf(comport, send_buffer, 1);
  sleep(3);
}

void sendStop(){
  send_buffer[0] = 'b';
  RS232_SendBuf(comport, send_buffer, SEND_CHARS);
}

//void walkToObject()

bool turnTo(string cascadef)
{
  int ori = -1;
  bool found = false;

  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return false;
  }

  cv::CascadeClassifier cascade;
  cascade.load(cascadef);

  Mat img;

  time_t end = time(NULL) +15 ;
  while(time(NULL) <= end && !found)
  {
    cap >> img;
    if(rotImage)
      cv::flip(img,img,-1); // to flip the camera uncomment this

    ori = image_whereIsCascade(img,cascade,false);
    //cout<<"Time right Now: "<<time(NULL)<< " Time we aim for: "<< end <<endl;
    if(cameraFeed){
      imshow("Image", img);
      cv::waitKey(1);
    }
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
  }
  send_buffer[0] = 'b';
  RS232_SendBuf(comport, send_buffer,1);
  return found;
  RS232_CloseComport(comport);

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

bool initConf(string file)
{
  INIReader reader(file);
  if (reader.ParseError() < 0) {
    std::cout << "Can't load 'test.ini'\n";
    return false;
  }
  //image
  cameraDevice = reader.GetInteger("image", "cameraDevice", cameraDevice);
  rotImage = reader.GetBoolean("image", "rotImage", rotImage);
  cameraFeed = reader.GetBoolean("image", "cameraFeed", cameraFeed);
  printDetectionTime = reader.GetBoolean("image", "printDetectionTime", printDetectionTime);
  cascadeFile = reader.Get("image", "cascadeFile", cascadeFile);
  //comm
  comport = reader.GetInteger("comm", "comport", comport);
  baudrate = reader.GetInteger("comm", "baudrate", baudrate);
}

void initAll(string file)
{
  initConf(file);
  image_initConf(file);
  audio_initConf(file);
}


bool cParser( int argc, char** argv )
{
  int opt;
  while ((opt = getopt (argc, argv, "c:h")) != -1)
  {
    switch (opt)
    {
      case 'c':
      configFile = atoi(optarg);
      break;
      break;
      case 'h':
      cout<<"-c [arg] for setting config-file"<<endl;
      cout<<endl;
      return false;
      break;
    }
  }
  //printSettings();
  return true;
}

void printSettings()
{
  cout<<"The program is run using these settings:"<<endl;
  cout<<"Capture device: "<<cameraDevice<<endl;
  cout<<"Cascade file: "<<cascadeFile<<endl;
  cout<<"Rotation: "<<rotImage<<endl;
  cout<<"Camera feed: "<<cameraFeed<<endl;
  cout<<"Print cascade detection time: "<<printDetectionTime<<endl;
  cout<<"Demo: "<<demo<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;
}
