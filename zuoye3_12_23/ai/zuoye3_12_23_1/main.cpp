

// #include <opencv2/opencv.hpp>
// #include <bits/stdc++.h>
// using namespace std;
// using namespace cv;
//     int maxv=255;
//     vector<vector<Point>> sur;
//     Mat picture=imread("../input/question1.jpg");
//     Mat gray_1,black;
//     Mat pic;

// // void recall_function(int a,void * b){
// //     Mat *k= (Mat *)b;
// //     adaptiveThreshold(*k,black,maxv,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
// //     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
// //     Mat result = picture.clone();
// //     drawContours(result, sur, -1, Scalar(200, 100, 100), 2);
// //     // drawContours(picture,sur,-1,Scalar(200,100,100),2);
// //     imshow("test",result);
// // }
// int main(){

//     picture=imread("../input/question1.jpg");
//     cvtColor(picture,gray_1,COLOR_BGR2GRAY);
//     adaptiveThreshold(gray_1,black,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
//     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//     pic=picture.clone();
//     drawContours(picture,sur,-1,Scalar(0,0,255),2);
//     std::string address="../output/coins/",nam="coin_";
//     for (int i=0;i<sur.size();i++){
//         double are=contourArea(sur[i]);
//         if(are<500){
//             continue;
//         }
//         Rect rect=boundingRect(sur[i]);
        
//         Mat new22=pic(rect);
//         string new_name=nam+to_string(i);
//         string new_add=address+new_name+".jpg";
//         imwrite(new_add,new22);
//         cout<<"New it is "<<i<<endl;
//     }
//     imwrite("../output/question1.jpg",picture);
//     imwrite("../output/gray.jpg",black);
//     return 0;
// }

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

// 全局变量（回调函数需用，简化新手操作，避免复杂传参）
Mat src, gray, binary, result;
vector<vector<Point>> contours;  // 所有图形外轮廓
vector<double> contour_areas;    // 每个轮廓的面积，一一对应
int max_area = 0;                // 图形最大面积（滑动条最大值）
const string WINDOW_NAME = "图形面积筛选+分类标注（滑动条调整面积）";

// 滑动条回调函数：面积阈值调整，实时筛选+标注（适配面积筛选需求）
void areaTrackbarCallback(int area_thresh, void*) {
    result = src.clone();  // 每次调整重置结果图
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contour_areas[i];
        vector<Point> cnt = contours[i];
        // 筛选：只显示 面积≥滑动条阈值 的图形
        if (area >= area_thresh) {
            // 绘制筛选后的图形轮廓（绿色，线宽2）
            drawContours(result, contours, i, Scalar(0, 255, 0), 2);
            // 获取图形中心（用于标注文字，避免遮挡图形）
            Moments m = moments(cnt);
            int cx = m.m10 / m.m00;  // 中心x坐标（无需中间量，直接用）
            int cy = m.m01 / m.m00;  // 中心y坐标
            Point center(cx, cy);

            // 核心：图形分类（通过轮廓近似判断形状）
            string shape = "未知图形";
            double eps = arcLength(cnt, true) * 0.02;  // 近似精度，越小越贴合轮廓
            vector<Point> approx;
            approxPolyDP(cnt, approx, eps, true);  // 轮廓近似为多边形
            int edge_num = approx.size();  // 多边形边数=图形近似边数

            // 边数对应图形分类（适配基础图形，可扩展）
            if (edge_num == 3) shape = "tri";
            else if (edge_num == 4) {
                // 正方形：邻边近似相等 + 内角近似90度
                Rect rect = boundingRect(approx);
                float ratio = (float)rect.width / rect.height;
                shape = (ratio > 0.95 && ratio < 1.05) ? "zheng" : "juxing";
            }
            else if (edge_num >= 6 && edge_num <= 10) shape = "six";
            else if (edge_num > 10) shape = "circle";  // 边数越多，越接近圆形

            // 标注：面积值+图形类别（白色文字，黑色描边，清晰不遮挡）
            string text = "area:" + to_string((int)area) + " | " + shape;
            putText(result, text, center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);  // 黑边
            putText(result, text, center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);  // 白字
        } else {
            // 未筛选的图形：标灰色轮廓，不标注文字
            drawContours(result, contours, i, Scalar(128, 128, 128), 1);
        }
    }
    imshow(WINDOW_NAME, result);
}

int main() {
    // 1. 读取图形图（替换为你的图形路径，三角形/正方形/圆形混合图最佳）
    src = imread("../input/shape.jpg");
    if (src.empty()) {
        cout << "无法读取图形图片，请检查路径！" << endl;
        return -1;
    }
    // gray = Mat(src.size(), CV_8UC1);
    // cvtColor(src, gray, COLOR_BGR2GRAY);
    // // 预处理：二值化+形态学优化（提轮廓精度，可选，根据图调整）
    // threshold(gray, binary, 100, 255, THRESH_BINARY_INV);
    // Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    // morphologyEx(binary, binary, MORPH_CLOSE, kernel);  // 闭运算：先膨胀后腐蚀，填补图形内部小孔

    // 2. 查找所有图形【外轮廓】
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // 计算每个轮廓的面积，并存入数组，同时找最大面积（滑动条最大值）
    contour_areas.resize(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
        contour_areas[i] = contourArea(contours[i]);
        if (contour_areas[i] > max_area) {
            max_area = (int)contour_areas[i];
        }
    }
    // 最大面积保底（避免滑动条无意义）
    if (max_area == 0) max_area = 1000;

    // 3. 创建窗口+绑定面积滑动条（滑动条名+窗口名+初始阈值+最大值+回调函数）
    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
    int init_thresh = 100;  // 滑动条初始阈值（可改）
    createTrackbar("面积筛选阈值", WINDOW_NAME, &init_thresh, max_area, areaTrackbarCallback);

    // 4. 初始化显示
    areaTrackbarCallback(init_thresh, nullptr);
    cout << "滑动条可调整面积范围：0~" << max_area << endl;
    cout << "图形分类规则：3边=三角形，4边=矩形/正方形，6~10边=六边形，>10边=圆形" << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}







