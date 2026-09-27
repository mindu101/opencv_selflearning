// // #include <opencv2/opencv.hpp>
// // #include <bits/stdc++.h>
// // using namespace std;
// // using namespace cv;
// //     int maxv=255;
// //     vector<vector<Point>> sur;
// //     Mat picture=imread("../input/question1.jpg");
// //     Mat gray_1,black;
// //     Mat pic;
// //     int area=0;
// //     vector<Point> p;

// // // void recall_function(int a,void * b){
// // //     Mat *k= (Mat *)b;
// // //     adaptiveThreshold(*k,black,maxv,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
// // //     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
// // //     Mat result = picture.clone();
// // //     drawContours(result, sur, -1, Scalar(200, 100, 100), 2);
// // //     // drawContours(picture,sur,-1,Scalar(200,100,100),2);
// // //     imshow("test",result);
// // // }
// // void recall(int a,void *b){
// //     Mat pci=picture.clone();
// //     drawContours(pci, sur, -1, Scalar(0,0,255), 2);
// //     for(int i=0;i<sur.size();i++){
// //         if(contourArea(sur[i])>a){
// //             putText(pci,to_string((int)contourArea(sur[i])),p[i],FONT_HERSHEY_SIMPLEX,1.0,Scalar(255,0,0),1,false);
// //         }
// //     }
// //     imshow("test",pci);
// // }
// // int main(){

// //     picture=imread("../input/shape.jpg");
// //     cvtColor(picture,gray_1,COLOR_BGR2GRAY);
// //     adaptiveThreshold(gray_1,black,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
// //     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
// //     pic=picture.clone();
// //     drawContours(picture,sur,-1,Scalar(0,0,255),2);
// //     p.resize(sur.size());
// //     for(int i=0;i<sur.size();i++){
// //         p[i].x=moments(sur[i]).m10/moments(sur[i]).m00;
// //         p[i].y=moments(sur[i]).m01/moments(sur[i]).m00;
// //         // p_area[i]=contourArea(sur[i]);
// //     }
// //     namedWindow("test",WINDOW_AUTOSIZE);
// //     createTrackbar("A","test",&area,76316,recall);
// //     recall(area,nullptr);
// //     waitKey(0);
// //     imwrite("../output/shape1.jpg",picture);
// //     imwrite("../output/gray.jpg",black);
// //     return 0;
// // }
// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include <vector>
// #include <filesystem>  // 用于创建文件夹，C++17及以上支持
// using namespace cv;
// using namespace std;
// namespace fs = std::filesystem;

// int main() {
//     // 1. 读取硬币图（替换为你的硬币图片路径，相对/绝对路径都可）
//     Mat src = imread("../input/question1.jpg");
//     if (src.empty()) {
//         cout << "无法读取图片，请检查路径！" << endl;
//         return -1;
//     }
//     Mat gray, binary, morph;
//     // 2. 预处理：转灰度+高斯模糊（去噪）
//     cvtColor(src, gray, COLOR_BGR2GRAY);
//     GaussianBlur(gray, gray, Size(3, 3), 0);  // 模糊去小颗粒噪点

//     // 3. 二值化：黑白分离，硬币（前景）白，背景黑（可调阈值适配你的图）
//     threshold(gray, binary, 120, 255, THRESH_BINARY_INV);  // 反二值化，硬币白背景黑更易检测

//     // 4. 实验1必用：腐蚀+膨胀（形态学操作，精准提外轮廓，解决噪点干扰）
//     // 结构元素：圆形（适配硬币圆形轮廓，比矩形效果好）
//     Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
//     erode(binary, morph, kernel);  // 第一步：腐蚀，去掉硬币边缘细小杂点
//     dilate(morph, morph, kernel);  // 第二步：膨胀，还原硬币原始轮廓大小

//     // 5. 查找硬币【外轮廓】：CV_RETR_EXTERNAL 只取最外层轮廓，排除内部杂轮廓
//     vector<vector<Point>> contours;  // 存储所有硬币外轮廓
//     findContours(morph, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//     // 创建output文件夹（不存在则自动创建，存放单个硬币）
//     string output_path = "../output/";
//     if (!fs::exists(output_path)) {
//         fs::create_directory(output_path);
//     }
//     int coin_count = 0;  // 硬币计数，用于命名
//     Mat result = src.clone();  // 绘制轮廓后的结果图（对应question1效果）

//     // 6. 挨个遍历轮廓：筛选+绘制+裁剪+存储
//     for (size_t i = 0; i < contours.size(); i++) {
//         // 筛选有效硬币：面积过滤（调阈值排除极小干扰，根据你的硬币大小改）
//         double area = contourArea(contours[i]);
//         if (area < 500) continue;  // 过滤小噪点轮廓

//         // 绘制硬币外轮廓（红色，线宽2，清晰可见）
//         drawContours(result, contours, i, Scalar(0, 0, 255), 2);
//         // 获取硬币外接矩形（精准裁剪单个硬币）
//         Rect rect = boundingRect(contours[i]);
//         // 裁剪单个硬币（从原图裁剪，保留彩色）
//         Mat coin_roi = src(rect);
//         // 挨个存入output，命名：coin_0.jpg、coin_1.jpg...
//         string coin_name = output_path + "coin_" + to_string(coin_count) + ".jpg";
//         imwrite(coin_name, coin_roi);
//         cout << "单个硬币已存：" << coin_name << endl;
//         coin_count++;
//     }

//     // 7. 显示结果（对应question1效果展示）
//     cout << "共检测到" << coin_count << "个硬币，全部存入output文件夹！" << endl;
//     imshow("硬币外轮廓检测效果（question1效果）", result);
//     imshow("形态学处理后（腐蚀+膨胀）", morph);
//     waitKey(0);
//     destroyAllWindows();
//     return 0;
// }

// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include <vector>
// #include <filesystem>
// using namespace cv;
// using namespace std;
// namespace fs = std::filesystem;

// int main() {
//     // 1. 读取你的欧元硬币原图（替换为实际图片路径）
//     Mat src = imread("../input/question1.jpg");
//     if (src.empty()) {
//         cout << "无法读取图片，请检查路径！" << endl;
//         return -1;
//     }
//     Mat gray, binary, morph;
//     // 2. 预处理：转灰度+中值滤波（去硬币纹理噪点，比高斯更适合）
//     cvtColor(src, gray, COLOR_BGR2GRAY);
//     medianBlur(gray, gray, 5);  // 中值滤波，消除硬币表面纹理干扰

//     // 3. 自适应二值化（关键优化：适配不同亮度的硬币）
//     adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, 
//                       THRESH_BINARY_INV, 11, 2);  // 11为块大小，2为常数，可微调

//     // 4. 形态学闭运算（先膨胀后腐蚀，填补硬币内部小孔，保留外轮廓）
//     Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));  // 圆形核适配硬币轮廓
//     morphologyEx(binary, morph, MORPH_CLOSE, kernel, Point(-1, -1), 2);  // 执行2次闭运算，增强效果

//     // 5. 查找最外层轮廓（只取外轮廓，排除内部杂点）
//     vector<vector<Point>> contours;
//     vector<Vec4i> hierarchy;
//     findContours(morph, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//     // 创建output文件夹（存放单个硬币）
//     string output_path = "../output/";
//     if (!fs::exists(output_path)) {
//         fs::create_directory(output_path);
//     }
//     int coin_count = 0;
//     Mat result = src.clone();

//     // 6. 遍历轮廓：精准筛选硬币+绘制外轮廓+存储
//     for (size_t i = 0; i < contours.size(); i++) {
//         // 计算轮廓面积+周长，过滤过小/过大的干扰
//         double area = contourArea(contours[i]);
//         double perimeter = arcLength(contours[i], true);
//         // 硬币的圆度特征：4π*面积/周长² ≈ 1（越接近1越圆）
//         double circularity = (4 * CV_PI * area) / (perimeter * perimeter);

//         // 筛选条件（针对你的欧元硬币，精准调整）
//         if (area > 1000 && area < 50000 && circularity > 0.7) {  // 面积范围+圆度筛选
//             // 轮廓逼近：去除微小毛刺，让外轮廓更平滑
//             vector<Point> approx;
//             double eps = perimeter * 0.01;
//             approxPolyDP(contours[i], approx, eps, true);

//             // 绘制硬币外轮廓（红色，线宽3，清晰可见）
//             drawContours(result, contours, i, Scalar(0, 0, 255), 3);
//             // 获取硬币外接矩形，裁剪单个硬币
//             Rect rect = boundingRect(contours[i]);
//             // 防止裁剪越界
//             rect.x = max(0, rect.x);
//             rect.y = max(0, rect.y);
//             rect.width = min(rect.width, src.cols - rect.x);
//             rect.height = min(rect.height, src.rows - rect.y);
//             Mat coin_roi = src(rect);

//             // 挨个存入output文件夹
//             string coin_name = output_path + "euro_coin_" + to_string(coin_count) + ".jpg";
//             imwrite(coin_name, coin_roi);
//             cout << "硬币" << coin_count << "已保存：" << coin_name << endl;
//             coin_count++;
//         }
//     }

//     // 7. 显示结果
//     cout << "共检测到" << coin_count << "个欧元硬币，已存入output文件夹！" << endl;
//     imshow("欧元硬币外轮廓检测效果", result);
//     imshow("二值化+形态学处理后", morph);
//     // 保存检测效果图（可选）
//     imwrite("coin_contour_result.jpg", result);

//     waitKey(0);
//     destroyAllWindows();
//     return 0;
// }
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
using namespace cv;
using namespace std;
namespace fs = std::filesystem;

int main() {
    // 1. 读取指定路径的硬币图：../input/question1.jpg
    string input_path = "../input/question1.jpg";
    Mat src = imread(input_path);
    if (src.empty()) {
        cout << "无法读取图片，请检查路径：" << input_path << endl;
        cout << "请确认input文件夹存在且question1.jpg在该目录下！" << endl;
        return -1;
    }

    Mat gray, binary, morph;
    // 2. 预处理：转灰度+中值滤波（消除硬币纹理干扰）
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 5);  // 中值滤波，适配欧元硬币表面纹理

    // 3. 自适应二值化（优化参数：块大小15，常数5，增强连通性）
    adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, 
                      THRESH_BINARY_INV, 15, 5);

    // 4. 距离变换+形态学优化（根治内部孔洞+硬币粘连问题）
    Mat dist, dist_norm, dist_thresh;
    distanceTransform(binary, dist, DIST_L2, 5);  // 距离变换分离粘连硬币
    normalize(dist, dist_norm, 0, 255, NORM_MINMAX);  // 归一化
    threshold(dist_norm, dist_thresh, 100, 255, THRESH_BINARY);  // 阈值分割填孔洞
    dist_thresh.convertTo(dist_thresh, CV_8U);  // 转8位单通道

    // 最终形态学闭运算，去除残留杂点
    Mat kernel_final = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(dist_thresh, morph, MORPH_CLOSE, kernel_final);

    // 5. 查找最外层轮廓（只提取硬币外轮廓）
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(morph, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 6. 创建指定输出路径：../output/（不存在则自动创建）
    string output_path = "../output/";
    if (!fs::exists(output_path)) {
        if (!fs::create_directories(output_path)) {  // 支持多级目录创建
            cout << "无法创建输出文件夹：" << output_path << endl;
            return -1;
        }
    }

    int coin_count = 0;
    Mat result = src.clone();

    // 7. 遍历轮廓：精准筛选欧元硬币+绘制外轮廓+存储
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        double perimeter = arcLength(contours[i], true);
        double circularity = (4 * CV_PI * area) / (perimeter * perimeter);  // 圆度筛选

        // 适配欧元硬币的筛选条件（面积+圆度，避免漏检/误检）
        if (area > 1000 && area < 50000 && circularity > 0.6) {
            // 轮廓逼近，去除微小毛刺
            vector<Point> approx;
            double eps = perimeter * 0.01;
            approxPolyDP(contours[i], approx, eps, true);

            // 绘制红色外轮廓（线宽3，清晰标注）
            drawContours(result, contours, i, Scalar(0, 0, 255), 3);

            // 获取硬币外接矩形并裁剪，防止越界
            Rect rect = boundingRect(contours[i]);
            rect.x = max(0, rect.x);
            rect.y = max(0, rect.y);
            rect.width = min(rect.width, src.cols - rect.x);
            rect.height = min(rect.height, src.rows - rect.y);
            Mat coin_roi = src(rect);

            // 保存单个硬币到../output/目录
            string coin_name = output_path + "euro_coin_" + to_string(coin_count) + ".jpg";
            if (!imwrite(coin_name, coin_roi)) {
                cout << "保存硬币" << coin_count << "失败！" << endl;
                continue;
            }
            cout << "硬币" << coin_count << "已保存：" << coin_name << endl;
            coin_count++;
        }
    }

    // 8. 保存检测结果图到输出目录
    string result_name = output_path + "coin_contour_result.jpg";
    imwrite(result_name, result);
    string morph_name = output_path + "morph_processed.jpg";
    imwrite(morph_name, morph);

    // 9. 显示结果
    cout << "========================" << endl;
    cout << "检测完成！共识别" << coin_count << "个欧元硬币" << endl;
    cout << "检测结果图：" << result_name << endl;
    cout << "形态学处理图：" << morph_name << endl;
    cout << "单个硬币存储路径：" << output_path << endl;
    imshow("欧元硬币外轮廓检测效果", result);
    imshow("二值化+形态学处理后", morph);

    waitKey(0);
    destroyAllWindows();
    return 0;
}










