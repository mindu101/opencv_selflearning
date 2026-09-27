#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
Mat ima=imread("../picture/original_image1.png"),imb=imread("../picture/original_image2.png"),gray_1;
Mat gray_2;
int maxvalue=255;
void recall_function(int a,void * b){
  Mat * han=(Mat *)b;
  adaptiveThreshold(*han,gray_2,maxvalue,1,1,7,0);
  imshow("trackbar",gray_2);
  
}
int main(){
  cvtColor(ima,gray_1,COLOR_BGR2GRAY);
  namedWindow("trackbar",WINDOW_AUTOSIZE);
  createTrackbar("A","trackbar",&maxvalue,255,recall_function,&gray_1);
  recall_function(maxvalue,&gray_1);
  waitKey(0);
  return 0;
}
