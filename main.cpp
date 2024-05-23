#include <iostream>
#include<opencv2/opencv.hpp> //Общий заголовок для  OpenCV
#include<opencv2/features2d.hpp> //Детекторы FAST, BRISK, ORB
#include <opencv2/xfeatures2d.hpp> //Экспериментальные или лицензированные детекторы (SIFT  SURF)
#include "video.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main() {
            VideoCapture cap("../Video/sample_mpg.avi");
            if (!cap.isOpened()) {
                cerr << "Нет видеофайла." << endl;
                return -1;
            }
            VideoCapture cap2("../Video/sample_mpg.avi");
            if (!cap.isOpened()) {
                cerr << "Нет видеофайла." << endl;
                return -1;
            }

    Video vid;
    vid.WithOutClahe(cap);

    vid.WithClahe(cap2);

    return 0;
}
