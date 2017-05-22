#include "image.h"
#include "../tools/INIReader.h"

using namespace cv;
using namespace std;

/*
width: 640
height: 480
*/

//Config
/*
int cameraDevice = 0;
bool rotImage = false;
bool cameraFeed = false;
bool printDetectionTime = false;*/

//Config for detectMultiScale
double scaleFactor = 1.3;
int minNeighbors = 5;
int flags = 0;
int minSizeW = 60;
int minSizeH = 60;
int maxSizeW = 400;
int maxSizeH = 400;
Size minSize(minSizeW,minSizeH);
Size maxSize(maxSizeW,maxSizeH);


double countRBG(Mat img, Vec3b rgb, double diff);


bool image_initConf(string file)
{
  INIReader reader(file);
  if (reader.ParseError() < 0) {
      std::cout << "Can't load 'test.ini'\n";
      return false;
  }
  /*
  cameraDevice = reader.GetInteger("image", "cameraDevice", cameraDevice);;
  rotImage = reader.GetBoolean("image", "rotImage", rotImage);
  cameraFeed = reader.GetBoolean("image", "cameraFeed", cameraFeed);
  printDetectionTime = reader.GetBoolean("image", "printDetectionTime", printDetectionTime);

*/
  //Multiscale
  scaleFactor = reader.GetReal("detectMultiScale", "scaleFactor", scaleFactor);
  minNeighbors = reader.GetInteger("detectMultiScale", "minNeighbors", minNeighbors);
  flags = reader.GetInteger("detectMultiScale", "flags", flags);
  minSizeW = reader.GetInteger("detectMultiScale", "minSizeW", minSizeW);
  minSizeH = reader.GetInteger("detectMultiScale", "minSizeH", minSizeH);
  maxSizeW = reader.GetInteger("detectMultiScale", "maxSizeW", maxSizeW);
  maxSizeH = reader.GetInteger("detectMultiScale", "maxSizeH", maxSizeH);
}
/*

void image_findCascade(CascadeClassifier& cascade, int& dir)
{
  VideoCapture cap(cameraDevice);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return;
  }
  Mat frame;
  while(1)
  {
    cap >> frame;
    if(rotImage)
      flip(frame, frame, -1);

    if(cameraFeed)
    {
      imshow("CameraFeed", frame);
      cv::waitKey(5);
    }

    dir = image_whereIsCascade(frame,cascade,printDetectionTime);
  }
}
*/
/*
NAME IS TEMP
This function finds the first cascade in the image and then returns it's
orientation based on an keypad. The orientation is based on the center of the
cascade.
@ARGS:
Image
cascade to detect
print boolean, if true prints time for detections
flip boolean, if true flips image before detections
returns:
4 = left
5 = middle
6 = right
-1 = error
*/
int image_whereIsCascade(Mat& img, CascadeClassifier& cascade, bool print)
{
  Rect detected = image_detectAndGet(img,cascade,print,1);

  if(detected.x == -1)
    return -1;

  int point = detected.x + detected.width/2;

  //Left side
  if(point < img.cols/3)
    return 4;
  //Middle
  if(point >= img.cols/3 && point <= ((img.cols/3)*2))
    return 5;
  //Right
  if(point > ((img.cols/3)*2))
    return 6;

  return -1;
}

int image_capture()
{
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

/*
  cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
*/

  Mat frame;
  while(1) {
    cap >> frame;

    if (frame.empty()) {
      cerr << "ERROR: Unable to grab from the camera" << endl;
      break;
    }
    if(1)
    {
      imshow("Live",frame);
      int key = cv::waitKey(5);
    }




  }

  cap.release();
  destroyAllWindows();
  return 0;
}

void image_detectAndDraw(Mat& img, CascadeClassifier& cascade, bool print)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );

  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray, detected, scaleFactor, minNeighbors, flags, minSize, maxSize);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}


  for(int i = 0; i < detected.size(); i++)
  {
    rectangle(img, detected[i], Scalar(255,0,0));
  }
}

int image_getNumDetections(Mat& img, CascadeClassifier& cascade, bool print)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );


  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray,detected, scaleFactor, minNeighbors, flags, minSize, maxSize);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}


  return detected.size();
}

/*
Detects given cascade and returns an vector containing the detected rectangles
@ARGS:
Image
cascade to detect
print boolean, if true prints time for detections
flip boolean, if true flips image before detections
*/
vector<Rect> image_detectAndGet(Mat& img, CascadeClassifier& cascade, bool print)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );

  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray,detected, scaleFactor, minNeighbors, flags, minSize, maxSize);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}

  return detected;
}

Rect image_detectAndGet(Mat& img, CascadeClassifier& cascade, bool print, int index)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );

  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray,detected, scaleFactor, minNeighbors, flags, minSize, maxSize);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}

  if(detected.size()>=index)
    return detected[index];
  return Rect(-1,-1,0,0);
}

bool image_isInside(Rect moving, Rect still, int xOffset = 0, int yOffset = 0)
{
  Point p = Point(moving.x + xOffset, moving.y + yOffset);
  return still.contains(p);
}

double countRBG(Mat img, Vec3b rgb, double diff)
{
  Mat thresh;
  thresh = img.clone();
  double matching = 0;
  for(int y=0;y<img.rows;y++)
  {
    for(int x=0;x<img.cols;x++)
    {
      if((img.at<Vec3b>(Point(x,y))[0] <= rgb[0] * (1+diff) && img.at<Vec3b>(Point(x,y))[0] >= rgb[0] * (1-diff)) &&
      (img.at<Vec3b>(Point(x,y))[1] <= rgb[1] * (1+diff) && img.at<Vec3b>(Point(x,y))[1] >= rgb[1] * (1-diff)) &&
      (img.at<Vec3b>(Point(x,y))[2] <= rgb[2] * (1+diff) && img.at<Vec3b>(Point(x,y))[2] >= rgb[2] * (1-diff)))
      {
        matching++;
      }
      else
      {
        thresh.at<Vec3b>(Point(x,y))[0] = 0;
        thresh.at<Vec3b>(Point(x,y))[1] = 0;
        thresh.at<Vec3b>(Point(x,y))[2] = 0;
      }
    }
  }
  return matching / (img.rows * img.cols);
}
