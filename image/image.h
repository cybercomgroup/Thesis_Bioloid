#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int capture(int, int, bool);
void detectAndDraw(Mat&, CascadeClassifier&, bool, bool);
vector<Rect> detectAndGet(Mat&, CascadeClassifier&, bool, bool);
bool isInside(Rect, Rect, int, int);
