#include "image.h"

using namespace cv;
using namespace std;


double countRBG(Mat img, Vec3b rgb, double diff);


int image_capture(int width, int height, bool rot)
{
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }


  cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

  Mat frame, hsv, hsv_s;
  cout << "Start grabbing, press a key on Live window to terminate" << endl;
  while(1) {
    cap >> frame;

    if(rot){
      flip(frame, frame, -1);
    }


    cvtColor(frame, hsv, COLOR_BGR2HSV);

    inRange(hsv, cv::Scalar(125, 125, 150), cv::Scalar(200, 200, 255), hsv_s);

    GaussianBlur(hsv_s, hsv_s, cv::Size(9, 9), 2, 2);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours( hsv_s, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for( int i = 0; i < contours.size(); i++ )
    {
      cout<<contours[0].size()<<endl;
      Scalar color = Scalar(0,0,255);
      drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    if (frame.empty()) {
      cerr << "ERROR: Unable to grab from the camera" << endl;
      break;
    }
    imshow("Live",frame);
    //    imshow("Hsv",hsv_s);
    //imshow("Contours",contours);


    int key = cv::waitKey(5);
    key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
    if (key>=0)
    break;
  }

  cout << "Closing the camera" << endl;
  cap.release();
  destroyAllWindows();
  cout << "bye!" <<endl;
  return 0;
}
void image_detectAndDraw(Mat& img, CascadeClassifier& cascade, bool print, bool flip)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );

  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray,detected, 1.3, 5);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}


  for(int i = 0; i < detected.size(); i++)
  {
    rectangle(img, detected[i], Scalar(255,0,0));
  }
  imshow( "result", img );
}

/*
Detects given cascade and returns an vector containing the detected rectangles
@ARGS:
Image
cascade to detect
print boolean, if true prints time for detections
flip boolean, if true flips image before detections
*/
vector<Rect> image_detectAndGet(Mat& img, CascadeClassifier& cascade, bool print, bool flip)
{
  vector<Rect> detected;
  double t = 0;
  Mat gray;

  cvtColor( img, gray, COLOR_BGR2GRAY );

  if(flip){
    cv::flip(gray, gray, -1);
  }

  if(print){t = (double)getTickCount();}
  cascade.detectMultiScale(gray, detected, 1.1, 10);
  if(print){t = (double)getTickCount() - t; printf( "detection time = %g ms\n", t*1000/getTickFrequency());}

  return detected;
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
  imshow("Thresh",thresh);
  return matching / (img.rows * img.cols);
}
