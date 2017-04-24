#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

int capture(int, int, bool);
void detectAndDraw(cv::Mat&, cv::CascadeClassifier&, bool, bool);
