#include "image.h"

int main( int argc, char** argv )
{
    bool rot = false;
    if(argc > 1){
      if(argv[1][0] == 'r'){
        rot = true;
      }
    }

    cv::Mat img;
    img = cv::imread("img.png");

    cv::CascadeClassifier cascade;
    cascade.load("face_cascade.xml");

    detectAndDraw(img,cascade,false,false);

    imshow( "kek", img );
}
