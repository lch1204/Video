#include "video.h"

Video::Video()
{

}

int Video::WithClahe(VideoCapture cap)
{
    // Определим частоту кадров на видео
    double rate = cap.get(cv::CAP_PROP_FPS);
    // Рассчитаем задержку в миллисекундах
    int delay = 1000 / rate;
    cout << "Frame rate of video is " << rate << endl;

    Mat frame1, frame2;
    bool stop = false;

    cap >> frame1;// Считываем первый кадр

    // Конвертируем кадр в оттенки серого
    Mat gray1;
    if (frame1.empty()) {
            std::cerr << "Ошибка: изображение не загружено или путь указан неверно." << std::endl;
            return -1;
        }
    cvtColor(frame1, gray1, CV_BGR2GRAY);
    //адаптивноt выравнивание яркости
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4); //Clip Limit — это максимальное значение для гистограммы каждого небольшого блока. Ограничение клиппинга предотвращает усиление шума. В данном случае значение установлено на 4, что является эмпирически подобранным значением для предотвращения переусиления контраст
    Mat clahe1;
    clahe->apply(gray1, clahe1);

    // Детекторы и экстракторы
    Ptr<Feature2D> detector = BRISK::create();
    Ptr<Feature2D> extractor = BRISK::create();
    //         Ptr<Feature2D> detector = SIFT::create();
    //         Ptr<Feature2D> extractor = SIFT::create();
    //        // Ptr<Feature2D> detector = SURF::create();
    //        // Ptr<Feature2D> extractor = SURF::create();

    vector<KeyPoint> keypoints1;
    Mat descriptors1;

    // Нахождение ключевых точек и построение описаний
    detector->detect(clahe1, keypoints1);
    extractor->compute(clahe1, keypoints1, descriptors1);

    // Координаты ключевых точек для отслеживания
    vector<Point2f> points1;
    for (const auto& kp : keypoints1) {
        points1.push_back(kp.pt);
    }


    vector<Point2f> points2; // Параметры для функции оптического потока
    vector<uchar> status;
    vector<float> err;

    // Переменная для накопления траектории
    Point2f trajectory = Point2f(0, 0);


    int trajectory_width = frame1.cols*2; // Ширина траектории
    int trajectory_height = frame1.rows * 3; // Высота траектории
    Mat img_trajectory = Mat::zeros(Size(trajectory_width, trajectory_height), CV_8UC3); // траектория
    img_trajectory = Scalar(255, 255, 255); // Белый фон

    // Начальная позиция в центре большого изображения траектории
    Point2f start_position(trajectory_width / 2, 0); // Начальная позиция в центре верхнего края
    Point2f previous_position = start_position;

    // Переменные для хранения границ траектории
    int min_x = trajectory_width, min_y = trajectory_height, max_x = 0, max_y = 0;

    while (!stop) {
        // Считываем следующий кадр
        bool result = cap.read(frame2);
        if (!result) {
            cerr << "Error: Could not read frame." << endl;
            break;
        }

        // Конвертируем кадр в оттенки серого
        Mat gray2;
        cvtColor(frame2, gray2, COLOR_BGR2GRAY);

        Mat clahe2;
        clahe->apply(gray2, clahe2); // Применение CLAHE для выравнивания яркости

        // Вычисление оптического потока для отслеживания ключевых точек
        calcOpticalFlowPyrLK(clahe1, clahe2, points1, points2, status, err);

        // Вычисление средней разницы координат
        Point2f delta(0, 0);
        int count = 0;
        for (size_t i = 0; i < points1.size(); i++) {
            if (status[i]) {
                delta += points2[i] - points1[i];
                count++;
            }
        }

        if (count > 0) {
            delta *= (1.0 / count);
        }

        // Обновление траектории
        trajectory += delta;

        // Вычисление текущей позиции на изображении траектории
        Point2f current_position = previous_position + delta;
        line(img_trajectory, previous_position, current_position, Scalar(0, 45, 120), 2);
        previous_position = current_position;

        // Обновление границ траектории
        min_x = min(min_x, (int)current_position.x);
        min_y = min(min_y, (int)current_position.y);
        max_x = max(max_x, (int)current_position.x);
        max_y = max(max_y, (int)current_position.y);

        // Отображение текущего кадра и траектории
        //                imshow("Current Frame", frame2);
        imshow("Trajectory", img_trajectory);

        // Обновление предыдущего кадра и ключевых точек
        clahe1 = clahe2.clone();
        points1 = points2;

        int key = waitKey(delay);
        if (key == 27) // Если нажата клавиша ESC
            stop = true;
    }

    // Обрезка изображения по границам траектории
    Rect bounding_rect(min_x, min_y, max_x - min_x, max_y - min_y);
    Mat cropped_img_trajectory = img_trajectory(bounding_rect);

    // Сохранение обрезанного изображения траектории в файл .jpeg
    imwrite("trajectory_clahe.jpg", cropped_img_trajectory);

    cap.release();
    destroyAllWindows();
}

void Video::WithOutClahe(VideoCapture cap)
{
    // Определим частоту кадров на видео
    double rate = cap.get(cv::CAP_PROP_FPS);
    // Рассчитаем задержку в миллисекундах
    int delay = 1000 / rate;
    cout << "Frame rate of video is " << rate << endl;

    Mat frame1, frame2;
    bool stop = false;

    cap >> frame1;// Считываем первый кадр

    // Конвертируем кадр в оттенки серого
    Mat gray1;
    cvtColor(frame1, gray1, CV_BGR2GRAY);
    //    //адаптивноt выравнивание яркости
    //    Ptr<CLAHE> clahe = createCLAHE();
    //    clahe->setClipLimit(4); //Clip Limit — это максимальное значение для гистограммы каждого небольшого блока. Ограничение клиппинга предотвращает усиление шума. В данном случае значение установлено на 4, что является эмпирически подобранным значением для предотвращения переусиления контраст
    //    Mat clahe1;
    //    clahe->apply(gray1, clahe1);

    // Детекторы и экстракторы
    Ptr<Feature2D> detector = BRISK::create();
    Ptr<Feature2D> extractor = BRISK::create();
    //         Ptr<Feature2D> detector = SIFT::create();
    //         Ptr<Feature2D> extractor = SIFT::create();
    //        // Ptr<Feature2D> detector = SURF::create();
    //        // Ptr<Feature2D> extractor = SURF::create();

    vector<KeyPoint> keypoints1;
    Mat descriptors1;

    // Нахождение ключевых точек и построение описаний
    detector->detect(gray1, keypoints1);
    extractor->compute(gray1, keypoints1, descriptors1);

    // Координаты ключевых точек для отслеживания
    vector<Point2f> points1;
    for (const auto& kp : keypoints1) {
        points1.push_back(kp.pt);
    }

    // Параметры для функции оптического потока
    vector<Point2f> points2;
    vector<uchar> status;
    vector<float> err;

    // Переменная для накопления траектории
    Point2f trajectory = Point2f(0, 0);

    // Изображение для отображения траектории
    int trajectory_width = frame1.cols; // Ширина изображения для траектории
    int trajectory_height = frame1.rows * 3; // Высота изображения для траектории
    Mat img_trajectory = Mat::zeros(Size(trajectory_width, trajectory_height), CV_8UC3);
    img_trajectory = Scalar(255, 255, 255); // Белый фон

    // Начальная позиция в центре большого изображения траектории
    Point2f start_position(trajectory_width / 2, 0); // Начальная позиция в центре верхнего края
    Point2f previous_position = start_position;

    // Переменные для хранения границ траектории
    int min_x = trajectory_width, min_y = trajectory_height, max_x = 0, max_y = 0;

    while (!stop) {
        // Считываем следующий кадр
        bool result = cap.read(frame2);
        if (!result) {
            cerr << "Error: Could not read frame." << endl;
            break;
        }

        // Конвертируем кадр в оттенки серого
        Mat gray2;
        cvtColor(frame2, gray2, COLOR_BGR2GRAY);

        //
        //        Mat clahe2;
        //        clahe->apply(gray2, clahe2); // Применение CLAHE для адаптивного выравнивания яркости

        calcOpticalFlowPyrLK(gray1, gray2, points1, points2, status, err);

        // Вычисление средней разницы координат
        Point2f delta(0, 0);
        int count = 0;
        for (size_t i = 0; i < points1.size(); i++) {
            if (status[i]) {
                delta += points2[i] - points1[i];
                count++;
            }
        }

        if (count > 0) {
            delta *= (1.0 / count);
        }

        // Обновление траектории
        trajectory += delta;

        // Вычисление текущей позиции на изображении траектории
        Point2f current_position = previous_position + delta;
        line(img_trajectory, previous_position, current_position, Scalar(0, 200, 15), 2); // Красная линия
        previous_position = current_position;


        min_x = min(min_x, (int)current_position.x);
        min_y = min(min_y, (int)current_position.y);// Обновление границ траектории
        max_x = max(max_x, (int)current_position.x);
        max_y = max(max_y, (int)current_position.y);

        imshow("Current Frame", frame2);
        imshow("Trajectory", img_trajectory);

        gray1 = gray2.clone();
        points1 = points2;

        int key = waitKey(delay);
        if (key == 27) // Если нажата клавиша ESC
            stop = true;
    }

    Rect bounding_rect(min_x, min_y, max_x - min_x, max_y - min_y);    // Обрезка изображения
    Mat cropped_img_trajectory = img_trajectory(bounding_rect);

    imwrite("trajectory.jpg", cropped_img_trajectory);

    cap.release();
    destroyAllWindows();
}
