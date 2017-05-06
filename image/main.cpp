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
    Mat img;
    //img = cv::imread("img.png");

    //Cascade
    cv::CascadeClassifier cascade;
    cascade.load("cascades/face_cascade.xml");

    //Capture
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
      cerr << "ERROR: Unable to open the camera" << endl;
      return 0;
    }

    vector<Rect> detected;
    int rWidth = 640/3;
    int rHeight = 480;
    int rLx =  0;
    int rMx =  ((640/2)-rWidth/2);
    int rRx =  (640-rWidth);
    int ry =  0;
    Rect rL = Rect(rLx,ry,rWidth,rHeight);//STATIC RECT
    Rect rM = Rect(rMx,ry,rWidth,rHeight);//STATIC RECT
    Rect rR = Rect(rRx,ry,rWidth,rHeight);//STATIC RECT
    bool turning = false;
    while(1)
    {
      cap >> img;

      if(rot){
        flip(img, img, -1);
      }

      //Detection
      detected = image_detectAndGet(img,cascade,false,false);
      //image_detectAndDraw(img,cascade,true,false);



      /*
      rectangle(img, rL, Scalar(255,255,255), -1);
      rectangle(img, rM, Scalar(0,0,0), -1);
      rectangle(img, rR, Scalar(255,255,255), -1);
      */

      //REMEMBER DETECTED 0
      for(int i = 0; i < detected.size(); i++)
      {
        if(image_isInside(detected[0],rM,detected[0].width/2,detected[0].height/2))
        {
            cout<<"Middle"<<endl;
            turning = false;
            //Break loop
        }
        else
        {
          if(!turning)
          {
            if(image_isInside(detected[0],rL,detected[0].width/2,detected[0].height/2))
              cout<<"Left"<<endl;
            if(image_isInside(detected[0],rR,detected[0].width/2,detected[0].height/2))
              cout<<"Right"<<endl;
            turning = true;
          }
        }
      }

      imshow( "result", img );

      int key = cv::waitKey(1);
      key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
      if (key>=0)
        break;
    }

    cap.release();
    return 0;
}
