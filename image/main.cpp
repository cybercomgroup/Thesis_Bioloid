#include "image.h"

using namespace std;
using namespace cv;


/*
width: 640
height: 480
*/


int main( int argc, char** argv )
{
    bool show,rot = false;
    if(argc > 1){
      if(argv[1][0] == 'r'){
        rot = true;
      }
      if(argv[2][0] == 's'){
        show = true;
      }
    }
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
      ori=image_whereIsCascade(frame, cascade, false, rot);
      if(show)
        imshow("Live",frame);

      cout << ori << endl;
      int key = cv::waitKey(5);
      key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
      if (key>=0)
      break;
    }
    cap.release();
    return 0;
}
