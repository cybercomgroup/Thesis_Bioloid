#ifndef image_INCLUDED
#define image_INCLUDED

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

bool image_initConf(string);
void image_findCascade(CascadeClassifier&, int&);
void image_detectAndDraw(Mat&, CascadeClassifier&, bool);
vector<Rect> image_detectAndGet(Mat&, CascadeClassifier&, bool);
Rect image_detectAndGet(Mat&, CascadeClassifier&, bool, int);
int image_whereIsCascade(Mat&, CascadeClassifier&, bool);
int image_getNumDetections(Mat& img, CascadeClassifier&, bool);
bool image_isInside(Rect, Rect, int, int);
int image_capture(int, int);

#endif
