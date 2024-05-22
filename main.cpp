//#include<opencv2/opencv.hpp> //Общий заголовок для  OpenCV
//#include<opencv2/features2d.hpp> //Детекторы FAST, BRISK, ORB
//#include <opencv2/xfeatures2d.hpp> //Экспериментальные или лицензированные детекторы (SIFT  SURF)

//using namespace std;
//using namespace cv;
//using namespace cv::xfeatures2d;

//int main()
//{
//        VideoCapture cap("../Video/sample_mpg.avi");
//        if (!cap.isOpened()) {
//            cerr << "Нет видеофайла." << endl;
//            return -1;
//        }
//    bool stop = false;
//    // Определим частоту кадров на видео
//    double rate = cap.get(cv::CAP_PROP_FPS);
//    // Рассчитаем задержку в миллисекундах
//    int delay = 1000 / rate;
//    cout << "Frame rate of video is " << rate << endl;

//    Mat frame1, frame2;

//    cap >> frame1; // Считываем первый кадр

//    while (!stop) {
//            // Считываем следующий кадр
//            bool result = cap.read(frame2);
//            if (!result) {
//                cerr << "Error: Could not read frame." << endl;
//                break;
//            }

//    // Конвертируем кадры в оттенки серого
//    Mat gray1, gray2;
//    cvtColor(frame1, gray1, COLOR_BGR2GRAY);
//    cvtColor(frame2, gray2, COLOR_BGR2GRAY);

//    // Детекторы и экстракторы
////        Ptr<Feature2D> detector = BRISK::create();
////        Ptr<Feature2D> extractor = BRISK::create();
//         Ptr<Feature2D> detector = SIFT::create();
//         Ptr<Feature2D> extractor = SIFT::create();
//        // Ptr<Feature2D> detector = SURF::create();
//        // Ptr<Feature2D> extractor = SURF::create();

//        vector<KeyPoint> keypoints1, keypoints2;
//        Mat descriptors1, descriptors2;

//        // Нахождение ключевых точек
//        detector->detect(gray1, keypoints1);
//        detector->detect(gray2, keypoints2);

//        // Построение описаний
//        extractor->compute(gray1, keypoints1, descriptors1);
//        extractor->compute(gray2, keypoints2, descriptors2);

//        // Сопоставление ключевых точек
//        BFMatcher matcher(NORM_L2);
//        vector<vector<DMatch>> knnMatches;
//        matcher.knnMatch(descriptors1, descriptors2, knnMatches, 2);

//        const float ratio_thresh = 0.75f;
//        vector<DMatch> goodMatches;
//        for (size_t i = 0; i < knnMatches.size(); i++) {
//            if (knnMatches[i][0].distance < ratio_thresh * knnMatches[i][1].distance) {
//                goodMatches.push_back(knnMatches[i][0]);
//            }
//        }

//        // Вывод результата сопоставления
//        Mat img_matches;
//        drawMatches(frame1, keypoints1, frame2, keypoints2, goodMatches, img_matches);
//        imshow("Good Matches with CLAHE", img_matches);

//        // Переходим к следующему кадру
//        frame1 = frame2.clone();

//        int key = waitKey(delay);
//        if (key == 27) // Если нажата клавиша ESC
//            stop = true;
//    }

//    cap.release();
//    destroyAllWindows();
//    return 0;
//}
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
