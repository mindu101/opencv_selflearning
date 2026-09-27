#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main(){
    Mat ima=imread("o.jpg");
    imshow("input",ima);
    waitKey(1000);
    destroyAllWindows();
    return 0;
    
}