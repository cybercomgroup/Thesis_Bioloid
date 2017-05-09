#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int image_capture(int, int);
void image_detectAndDraw(Mat&, CascadeClassifier&, bool);
vector<Rect> image_detectAndGet(Mat&, CascadeClassifier&, bool);
Rect image_detectAndGet(Mat&, CascadeClassifier&, bool, int);
bool image_isInside(Rect, Rect, int, int);
string image_findCascade(Mat&, CascadeClassifier&, bool);
int image_whereIsCascade(Mat&, CascadeClassifier&, bool);
int image_getDetections(CascadeClassifier&);
