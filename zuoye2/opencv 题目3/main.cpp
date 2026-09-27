#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
int main(){
  Mat ima=imread("../picture/original_image1.png"),imb=imread("../picture/original_image2.png"),imc;
  Mat res;
  resize(ima,imc,Size(0,0),0.5,0.5,INTER_CUBIC);
  resize(imc,res,Size(imc.cols*2,imc.rows*2),0,0,INTER_NEAREST);
  imwrite("../picture/small.png",imc);
  imwrite("../picture/normal.png",res);
  line(ima,Point(0,0),Point(ima.cols,ima.rows),Scalar(0,0,255),3);
  imwrite("../picture/line.png",ima);
  circle(ima,Point(ima.cols/2,ima.rows/2),14,Scalar(0,255,0),FILLED);
  imwrite("../picture/line_and_circle.png",ima);
  Mat mid;
  resize(res,mid,Size(ima.cols,ima.rows),0,0);
  Mat new3;
  hconcat(mid,ima,new3);
  imwrite("../picture/result.png",new3);
  imshow("test",new3);
  waitKey(0);
  return 0;
}

