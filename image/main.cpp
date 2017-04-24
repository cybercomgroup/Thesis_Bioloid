#include "image.h"

using namespace std;
using namespace cv;


/*
width: 640
height: 480
*/

int main( int argc, char** argv )
{
    bool rot = false;
    if(argc > 1){
      if(argv[1][0] == 'r'){
        rot = true;
      }
    }

    //Image
    cv::Mat img;
    img = cv::imread("img.png");

    //Cascade
    cv::CascadeClassifier cascade;
    cascade.load("face_cascade.xml");

    //Capture
    cv::VideoCapture cap(0);
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
      vector<Rect> detected = detectAndGet(img,cascade,false,false);

      int rWidth = 100;
      int rHeight = 480;
      int rx =  ((640/2)-rWidth/2);
      int ry =  0;
      Rect r = Rect(rx,ry,rWidth,rHeight);//STATIC RECT

      rectangle(img, r, Scalar(0,0,0), -1);

      for(int i = 0; i < detected.size(); i++)
      {
        rectangle(img, detected[i], Scalar(255,0,0));
        cout<<isInside(detected[0],r,rWidth/2,rHeight/2)<<endl;
      }

      imshow( "result", img );

      int key = cv::waitKey(1);
      key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
      if (key>=0)
        break;
    }
    return 0;
}
