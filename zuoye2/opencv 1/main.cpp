#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
int main(){
  Mat ima=imread("../picture/original_image1.png"),imb,imc;
  Mat ima0;

  vector<Mat> chan;
  split(ima,chan);

  imwrite("../picture/B.png",chan[0]);
  imwrite("../picture/G.png",chan[1]);
  imwrite("../picture/R.png",chan[2]);
  Mat ouq;
  chan[1]=cv::Mat::zeros(chan[1].size(),chan[1].type());
  cv::merge(chan,ouq);
  imwrite("../picture/out_G=0.png",ouq);

  
  return 0;
}
