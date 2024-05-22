#ifndef VIDEO_H
#define VIDEO_H
#include<opencv2/opencv.hpp> //Общий заголовок для  OpenCV
#include<opencv2/features2d.hpp> //Детекторы FAST, BRISK, ORB
#include <opencv2/xfeatures2d.hpp> //Экспериментальные или лицензированные детекторы (SIFT  SURF)


using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

class Video
{
public:
    Video();
    int WithClahe(VideoCapture cap);

    void WithOutClahe(VideoCapture cap);
};

#endif // VIDEO_H
