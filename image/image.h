#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int image_capture(int, int, bool);
void image_detectAndDraw(Mat&, CascadeClassifier&, bool, bool);
vector<Rect> image_detectAndGet(Mat&, CascadeClassifier&, bool, bool);
Rect image_detectAndGet(Mat&, CascadeClassifier&, bool, bool, int);
bool image_isInside(Rect, Rect, int, int);
string image_findCascade(Mat&, CascadeClassifier&, bool, bool);
int image_whereIsCascade(Mat&, CascadeClassifier&, bool, bool);
int image_getDetections(CascadeClassifier&);
