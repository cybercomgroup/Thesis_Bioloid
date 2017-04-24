#include "rs232.h"
#include "image/image.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;
using namespace cv;

#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

void manualMode();

//Parser and global belonging variables
void cParser(int argN, char *argv[]);

bool test = false;
bool rot = false;
int comport = 16; //pNUMBER
int baudrate = 57600; //bNUMBER

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
  cout<<"Testmode: "<<test<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;


  unsigned char receive_buffer[RECEIVE_CHARS];
  unsigned char send_byte = 42;
  unsigned char send_buffer[SEND_CHARS];

  send_buffer[0] = 'p';

  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    if(test)
    {
      manualMode();
    }
    else
    {
      while(1)
      {
        //Main loop:
        Mat img;
        img = cv::imread("image/img.png");

        //Cascade
        CascadeClassifier cascade;
        cascade.load("image/face_cascade.xml");

        VideoCapture cap(0);
        if (!cap.isOpened()) {
          cerr << "ERROR: Unable to open the camera" << endl;
          return 0;
        }
        while(1)
        {
          cap >> img;

          if(rot){
            flip(img, img, -1);
          }

          //Detection
          vector<Rect> detected = detectAndGet(img,cascade,true,false);

          int rWidth = 100;
          int rHeight = 480;
          int rx =  ((640/2)-rWidth/2);
          int ry =  0;
          Rect r = Rect(rx,ry,rWidth,rHeight);//STATIC RECT

          rectangle(img, r, Scalar(0,0,0), -1);

          for(int i = 0; i < detected.size(); i++)
          {
            rectangle(img, detected[i], Scalar(255,0,0));
            if(isInside(detected[0],r,detected[0].width/2,detected[0].height/2)){RS232_SendBuf(comport, send_buffer, SEND_CHARS);}
          }

          //imshow( "result", img );

          int key = cv::waitKey(1);
          key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
          if (key>=0)
            break;
        }
      }
    }
    RS232_CloseComport(comport);
  }

  //RS232_CloseComport(comport);


  return 0;
}

void manualMode()
{
  int c;
  unsigned char receive_buffer[RECEIVE_CHARS];
  unsigned char send_buffer[SEND_CHARS];
  unsigned char send_byte = 42;
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
          cout<<"Write buffer to send:"<<endl;
          cin.clear();
          cin >> send_buffer;
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
          cout<<"buffer: "<<send_buffer<<endl;
        break;
        case 2:
          cout<<"Write byte to send:"<<endl;
          cin >> send_buffer;
          RS232_SendByte(comport, send_byte);
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
          test = true;
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
