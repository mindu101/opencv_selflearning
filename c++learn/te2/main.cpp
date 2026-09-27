#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
  VideoCapture camera("/dev/video0",CAP_V4L2);
  Mat picture;
  camera>>picture;
  imshow("1",picture);
  imwrite("t1.jpg",picture);
  waitKey(0);
  return 0;
  
  
}
