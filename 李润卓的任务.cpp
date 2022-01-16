#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
    Mat img = imread("4.jpeg");
    //读取图片
    Mat gray;//创建一个变量
    cvtColor(img, gray, COLOR_BGR2GRAY);//灰度图图化
    vector<Mat>channels;
    split(img, channels);
    Mat blue = channels.at(0);
    Mat binary;
    threshold(blue, binary, 200, 255, THRESH_BINARY);
    //Mat canny;
    //Canny(gray, canny, 500, 200, 3);
    Mat dilate_image, erode_image;
    Mat elementX = getStructuringElement(0, Size(25, 1));
    Mat elementY = getStructuringElement(0, Size(1, 19));
    Point point(-1, -1);
   dilate(binary, dilate_image, elementX,point,2);
   erode(dilate_image, erode_image, elementX, point, 4);
   dilate(erode_image, dilate_image, elementX, point, 2);
   erode(dilate_image, erode_image, elementY, point, 1);
   dilate(erode_image, dilate_image, elementY, point, 2);
   imwrite("dilate_image.jpg",dilate_image);
   Mat blurr_image;
   medianBlur(dilate_image, blurr_image, 15);
   medianBlur(blurr_image, blurr_image, 15);
    vector<vector<Point>>contours;
    findContours(blurr_image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>>new_contours;
    for (int i = 0; i < contours.size();i++) {
        double area =contourArea (contours[i]);
        if (area >7500) {
            continue;
        }
        if (area < 3000) {
            continue;
        }
        Rect rect = boundingRect(contours[i]);
        //rectangle(img, rect, Scalar(0, 255, 0), 5);
        RotatedRect box = minAreaRect(contours[i]);
        vector<Point2f>box_points(4);
        box.points(box_points.data());
        for (int j = 0;j < 4;j++) {
            line(img, box_points[j], box_points[(j + 1) % 4], Scalar(0, 0, 255), 5);
        }
        new_contours.emplace_back(contours[i]);
       }
    //drawContours(img, new_contours, -1, Scalar(0, 0, 255), 5);
    namedWindow("img", 0);// 
    resizeWindow("img", img.size());//改变窗口大小
    imshow("img", img);//显示
    waitKey(0);
    return 0;
}