#include "image.h"

using namespace cv;
using namespace std;

int capture(int width, int height)
{
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

  Mat frame;
  cout << "Start grabbing, press a key on Live window to terminate" << endl;
  while(1) {
    cap >> frame;
    double fps = cap.get(CAP_PROP_FPS);
    cout<<fps<<endl;
    Vec3b color;
    color.val[0] = 0;
    color.val[1] = 0;
    color.val[2] = 0;
    for(int y=0;y<40;y++)
    {
      for(int x=0;x<width;x++)
      {

        frame.at<Vec3b>(Point(x,y)) = color;
      }
    }

    if (frame.empty()) {
        cerr << "ERROR: Unable to grab from the camera" << endl;
        break;
    }
    //imshow("Live",frame);
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
