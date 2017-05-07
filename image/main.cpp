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
    //Cascade
    cv::CascadeClassifier cascade;
    cascade.load("cascades/face_cascade.xml");
    cout<<image_findCascade(cascade, rot)<<endl;

    return 0;
}
