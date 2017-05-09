#include "image.h"

using namespace std;
using namespace cv;


/*
width: 640
height: 480
*/
void detectionTest(Mat& image, CascadeClassifier& cascade);
void orientationTest(Mat& image, CascadeClassifier& cascade);


int main( int argc, char** argv )
{
    //image_capture(640,480,false);
    //Cascade
    VideoCapture cap(0);
    if (!cap.isOpened()) {
      cerr << "ERROR: Unable to open the camera" << endl;
      return 0;
    }

    cv::CascadeClassifier cascade;
    cascade.load("cascades/face_cascade.xml");

    int ori;
    Mat frame;
    while(1) {
      cap >> frame;
      orientationTest(frame, cascade);
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
  image_detectAndDraw(image, cascade, true, false);
}

void orientationTest(Mat& image, CascadeClassifier& cascade)
{
  int ori;
  ori = image_whereIsCascade(image, cascade, true, false);

  if(ori == 4)
    cout<<"left"<<endl;
  if(ori == 5)
    cout<<"middle"<<endl;
  if(ori == 6)
    cout<<"right"<<endl;
}
