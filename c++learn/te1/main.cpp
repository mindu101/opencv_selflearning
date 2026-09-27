#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
Mat a=imread("test.jpg");
imshow("test",a);
waitKey(1000);
Mat b;
cvtColor(a,b,COLOR_BGR2GRAY);
imshow("T1",b);
waitKey(1000);
imwrite("answer/gray.jpg",b);
Mat c;
cvtColor(a,c,COLOR_BGR2HSV);
imwrite("answer/hsv.jpg",b);
imshow("T2",b);
waitKey(1000);
return 0;
} 
