#include "rs232.h"
#include "image/image.h"
#include "audio/voce.h"

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

//Parser and global belonging variables
void cParser(int argN, char *argv[]);

bool man = false;
bool rot = false;
bool demo = false;
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
    cParser((argc),argv);
  }

  cout<<"The program is running these values:"<<endl;
  cout<<"Demo: "<<demo<<endl;
  cout<<"Manualmode: "<<man<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;

  if(demo)
    demoVoice();

  return 0;
}

void demoImage()
{
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    while(1)
    {
      //Main loop:
      Mat img;
      img = cv::imread("image/img.png");

      //Cascade
      CascadeClassifier cascade;
      cascade.load("image/cascades/face_cascade.xml");

      VideoCapture cap(0);
      if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return;
      }
      while(1)
      {
        cap >> img;

        if(rot){
          flip(img, img, -1);
        }

        //Detection
        vector<Rect> detected = image_detectAndGet(img,cascade,true,false);

        int rWidth = 100;
        int rHeight = 480;
        int rx =  ((640/2)-rWidth/2);
        int ry =  0;
        Rect r = Rect(rx,ry,rWidth,rHeight);//STATIC RECT

        rectangle(img, r, Scalar(0,0,0), -1);

        for(int i = 0; i < detected.size(); i++)
        {
          rectangle(img, detected[i], Scalar(255,0,0));
          send_buffer[0] = 'p';
          if(image_isInside(detected[0],r,detected[0].width/2,detected[0].height/2)){RS232_SendBuf(comport, send_buffer, SEND_CHARS);}
        }

        //imshow( "result", img );

        int key = cv::waitKey(1);
        key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
        if (key>=0)
        break;
      }
    }
    RS232_CloseComport(comport);
  }
}

void demoVoice()
{
  string s = "";
  bool quit;
  voce::init("audio/lib", false, true, "audio/grammar", "control");
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    while(!quit)
    {
      while (voce::getRecognizerQueueSize() > 0)
      {
        s = voce::popRecognizedString();
        cout<<"You said: "<< s <<endl;
        if(!s.compare("left"))
        {
          send_buffer[0] = 'a';
          RS232_SendBuf(comport, send_buffer, 1);
        }
        else if(!s.compare("right"))
        {
          send_buffer[0] = 'd';
          RS232_SendBuf(comport, send_buffer, 1);
        }
        else if(!s.compare("stop"))
        {
          send_buffer[0] = 'b';
          RS232_SendBuf(comport, send_buffer, 1);
        }

        int key = cv::waitKey(1);
        key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
        if (key>=0)
        quit = true;
      }
    }
    RS232_CloseComport(comport);
  }
  voce::destroy();
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

void cParser(int argN, char *argv[])
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
      case 't':
      man = true;
      break;

      case 'd':
      demo = true;
      break;

      case 'p':
      comport = number;
      break;

      case 'b':
      baudrate = number;
      break;

      case 'r':
      rot = true;
      break;

      default:
      break;
    }
  }
}
