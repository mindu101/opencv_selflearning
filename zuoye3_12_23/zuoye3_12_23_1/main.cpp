

#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
    int maxv=255;
    vector<vector<Point>> sur;
    Mat picture=imread("../input/question1.jpg");
    Mat gray_1,black;
    Mat pic;

// void recall_function(int a,void * b){
//     Mat *k= (Mat *)b;
//     adaptiveThreshold(*k,black,maxv,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
//     findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//     Mat result = picture.clone();
//     drawContours(result, sur, -1, Scalar(200, 100, 100), 2);
//     // drawContours(picture,sur,-1,Scalar(200,100,100),2);
//     imshow("test",result);
// }
int main(){

    picture=imread("../input/question1.jpg");
    cvtColor(picture,gray_1,COLOR_BGR2GRAY);
    adaptiveThreshold(gray_1,black,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,0);
    findContours(black,sur,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    pic=picture.clone();
    drawContours(picture,sur,-1,Scalar(0,0,255),2);
    std::string address="../output/coins/",nam="coin_";
    for (int i=0;i<sur.size();i++){
        double are=contourArea(sur[i]);
        if(are<500){
            continue;
        }
        Rect rect=boundingRect(sur[i]);
        
        Mat new22=pic(rect);
        string new_name=nam+to_string(i);
        string new_add=address+new_name+".jpg";
        imwrite(new_add,new22);
        cout<<"New it is "<<i<<endl;
    }
    imwrite("../output/question1.jpg",picture);
    imwrite("../output/gray.jpg",black);
    return 0;
}








