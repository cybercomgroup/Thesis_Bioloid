#include "image.h"
#include <getopt.h>

using namespace std;
using namespace cv;


/*
width: 640
height: 480
*/
//Globals, these are the defaults:
int cameraDevice = 0;
int demo = 0;
bool rotImage = false;
bool cameraFeed = true;
bool printDetectionTime = true;
bool printRunTime = false;
string cascadeFile = "face_cascade.xml";


bool cParser( int argc, char** argv );
void printSettings();

void detectionTest(Mat& image, CascadeClassifier& cascade);
void orientationTest(Mat& image, CascadeClassifier& cascade);
bool numDetectionsTest(Mat& img, CascadeClassifier& cascade);


int main( int argc, char** argv )
{
  if(!cParser(argc, argv))
    return 0;

  double t = 0;

  VideoCapture cap(cameraDevice);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  cv::CascadeClassifier cascade;
  cascade.load("cascades/" + cascadeFile);

  Mat frame;
  bool loop = true;
  while(loop) {
    if(printRunTime){t = (double)getTickCount();}
    cap >> frame;

    switch(demo)
    {
      case 1:
        detectionTest(frame, cascade);
      break;
      case 2:
        orientationTest(frame, cascade);
      break;
      case 3:
        cameraFeed = false;
        loop = numDetectionsTest(frame, cascade);
      break;
      default:
      break;
    }
    if(cameraFeed)
      imshow("Live",frame);

    int key = cv::waitKey(5);
    key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
    if (key>=0)
    break;
    if(printRunTime){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}
  }
  cap.release();
  return 0;
}

void detectionTest(Mat& image, CascadeClassifier& cascade)
{
  image_detectAndDraw(image, cascade, printDetectionTime);
}

void orientationTest(Mat& image, CascadeClassifier& cascade)
{
  int ori;
  ori = image_whereIsCascade(image, cascade, printDetectionTime);

  if(ori == 4)
    cout<<"left"<<endl;
  if(ori == 5)
    cout<<"middle"<<endl;
  if(ori == 6)
    cout<<"right"<<endl;
}

bool numDetectionsTest(Mat& img, CascadeClassifier& cascade)
{
  cout<<"Press any button to run detection, 0 exits"<<endl;
  int c;
  cout<<"Detections in camera feed: " <<image_getNumDetections(img, cascade, printDetectionTime)<<endl;
  fflush(stdin);
  cin>>c;
  if(c==0)
    return false;
}

bool cParser( int argc, char** argv )
{
  int opt;
  while ((opt = getopt (argc, argv, "c:srpd:f:h")) != -1)
  {
    switch (opt)
    {
      case 'c':
        cameraDevice = atoi(optarg);
        break;
      case 's':
        cameraFeed = !cameraFeed;
        break;
      case 'r':
        rotImage = !rotImage;
        break;
      case 'p':
        printDetectionTime = !printDetectionTime;
        break;
      case 'd':
        demo = atoi(optarg);
        break;
      case 'f':
        cascadeFile = optarg;
        break;
      case 'h':
        cout<<"-c [arg] capture device"<<endl;
        cout<<"-f [arg] cascade file inside /cascades"<<endl;
        cout<<"-r rotates the camera feed"<<endl;
        cout<<"-s show camera feed to user"<<endl;
        cout<<"-p turn on/off detection time prints"<<endl;
        cout<<"-d [arg] set what demo to use"<<endl;
        cout<<endl;

        cout<<"Available demos:"<<endl;
        cout<<"1 - detectionTest"<<endl;
        cout<<"2 - orientationTest"<<endl;
        cout<<"3 - numDetectionsTest"<<endl;
        cout<<endl;
        printSettings();
        return false;
        break;
    }
  }
  printSettings();
  return true;
}

void printSettings()
{
  cout<<"The program currently has the settings:"<<endl;
  cout<<"Capture device: "<<cameraDevice<<endl;
  cout<<"Cascade file: "<<cascadeFile<<endl;
  cout<<"Rotation: "<<rotImage<<endl;
  cout<<"Camera feed: "<<cameraFeed<<endl;
  cout<<"Print cascade detection time: "<<printDetectionTime<<endl;
  cout<<"Demo: "<<demo<<endl;
}
