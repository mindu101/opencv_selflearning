#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
int main(){
  Mat ima1=imread("../picture/original_image1.png"),ima2=imread("../picture/original_image2.png"),imc;
  Mat imc2,imc3,ima4,ima5;
  bitwise_and(ima1,ima2,imc);
  bitwise_or(ima1,ima2,imc2);
  bitwise_xor(ima1,ima2,imc3);
  bitwise_not(imc3,ima4);
  bitwise_not(imc2,ima5);
  imshow("te1",imc);
  waitKey(0);
  imwrite("../picture/out/and.png",imc);
  imshow("te1",imc2);
  waitKey(0);
  imwrite("../picture/out/or.png",imc2);
  imshow("te1",imc3);
  waitKey(0);
  imwrite("../picture/out/xor.png",imc3);
  imshow("te1",ima4);
  waitKey(0);
  imwrite("../picture/out/not_xor.png",ima4);
  imshow("te1",ima5);
  waitKey(0);
  imwrite("../picture/out/not_or.png",ima5);



  waitKey(0);
  return 0;
}
