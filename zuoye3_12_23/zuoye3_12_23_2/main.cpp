#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
    int maxv=255;
    vector<vector<Point>> sur;
    Mat picture=imread("../input/question1.jpg");
    Mat gray_1,black;
    Mat pic;
    int area=0;
    vector<Point> p;

// void recall_function(int a,void * b){
//     Mat *k= (Mat *)b;
//     adaptiveThreshold(*k,black,maxv,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
//     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//     Mat result = picture.clone();
//     drawContours(result, sur, -1, Scalar(200, 100, 100), 2);
//     // drawContours(picture,sur,-1,Scalar(200,100,100),2);
//     imshow("test",result);
// }
void recall(int a,void *b){
    Mat pci=picture.clone();
    drawContours(pci, sur, -1, Scalar(0,0,255), 2);
    for(int i=0;i<sur.size();i++){
        if(contourArea(sur[i])>a){
            putText(pci,to_string((int)contourArea(sur[i])),p[i],FONT_HERSHEY_SIMPLEX,1.0,Scalar(255,0,0),1,false);
        }
    }
    imshow("test",pci);
}
int main(){

    picture=imread("../input/shape.jpg");
    cvtColor(picture,gray_1,COLOR_BGR2GRAY);
    adaptiveThreshold(gray_1,black,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
    findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    pic=picture.clone();
    drawContours(picture,sur,-1,Scalar(0,0,255),2);
    p.resize(sur.size());
    for(int i=0;i<sur.size();i++){
        p[i].x=moments(sur[i]).m10/moments(sur[i]).m00;
        p[i].y=moments(sur[i]).m01/moments(sur[i]).m00;
        // p_area[i]=contourArea(sur[i]);
    }
    namedWindow("test",WINDOW_AUTOSIZE);
    createTrackbar("A","test",&area,76316,recall);
    recall(area,nullptr);
    waitKey(0);
    imwrite("../output/shape1.jpg",picture);
    imwrite("../output/gray.jpg",black);
    return 0;
}








