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


void cParser( int argc, char** argv );

void detectionTest(Mat& image, CascadeClassifier& cascade);
void orientationTest(Mat& image, CascadeClassifier& cascade);
void numDetectionsTest(Mat& img, CascadeClassifier& cascade);


int main( int argc, char** argv )
{
  cParser(argc, argv);


  VideoCapture cap(cameraDevice);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  cv::CascadeClassifier cascade;
  cascade.load("cascades/face_cascade.xml");

  Mat frame;
  while(1) {
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
        numDetectionsTest(frame, cascade);
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

void numDetectionsTest(Mat& img, CascadeClassifier& cascade)
{
  cout<<"Press any button to run detection, 0 exits"<<endl;
  int c = 1;
  while(c != 0)
  {
      cout<<"Detections in camera feed: " <<image_getNumDetections(img, cascade, printDetectionTime)<<endl;
      cin>>c;
  }
}

void cParser( int argc, char** argv )
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
      case 'h':
        cout<<"-c [arg] capture device"<<endl;
        cout<<"-f rotates the camera feed"<<endl;
        cout<<"-s show camera feed to user"<<endl;
        cout<<"-p turn on/off detection time prints"<<endl;
        cout<<"-d [arg] set what demo to use"<<endl;

        cout<<"Available demos:"<<endl;
        cout<<"1 - detectionTest"<<endl;
        cout<<"2 - orientationTest"<<endl;
        cout<<"3 - numDetectionsTest"<<endl;
        cout<<endl;
        break;
    }
  }
  cout<<"The program is run using these settings:"<<endl;
  cout<<"Capture device: "<<cameraDevice<<endl;
  cout<<"Rotation: "<<rotImage<<endl;
  cout<<"Camera feed: "<<cameraFeed<<endl;
  cout<<"Print cascade detection time: "<<printDetectionTime<<endl;
  cout<<"Demo: "<<demo<<endl;

  cout<<"Press any button to exit the program"<<endl;
}
