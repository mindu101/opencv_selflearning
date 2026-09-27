#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
Mat pic1,pic1_gray,mohu,pic_can,pic_hsv,pic_hsv_BGR,pic_0,pic_1;
Mat gray_canny,gray_blur;
vector<vector<Point>> pic_contour;
vector<Mat> hsv(3);
uchar Poa,Pob;
void dfs(int col,int row){
    if(col<0||row<0)return ;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            int y=col+i,x=row+j;
            if(y<pic1.cols&&x<pic1.rows&&x>=0&&y>=0){
                Poa=pic1.at<uchar>(y,x);
                Pob=pic1_gray.at<uchar>(y,x); 
                cout<<x<<","<<y<<":"<<(int)Poa<<"  "<<(int)Pob<<" ";
            }
        }
        cout<<endl;
    }
}
int count2=0;
void save(){
    imwrite("../save/灰度图"+to_string(count2++)+".jpg",pic1_gray);
    imwrite("../save/边缘"+to_string(count2++)+".jpg",pic_can);
    imwrite("../save/hsv"+to_string(count2++)+".jpg",pic_hsv);
    imwrite("../save/更改亮度"+to_string(count2++)+".jpg",pic_hsv_BGR);
    imwrite("../save/上下反转"+to_string(count2++)+".jpg",pic_0);
    imwrite("../save/左右反转"+to_string(count2++)+".jpg",pic_1);
    imwrite("../save/灰度图_模糊"+to_string(count2++)+".jpg",gray_blur);
    imwrite("../save/灰度图_边缘"+to_string(count2++)+".jpg",gray_canny);


}
int main(){
    pic1=imread("../picture/t1.jpg");
    //输出基本信息
    cout<<(pic1.type())<<endl<<"长："<<pic1.cols<<" 宽： "<<pic1.rows;
    //转为灰度图
    cvtColor(pic1,pic1_gray,COLOR_BGR2GRAY);
    //模糊
    blur(pic1,mohu,Size(3,3));
    Canny(pic1,pic_can,1,100);
    //组合：
    blur(pic1_gray,gray_blur,Size(3,3));
    Canny(pic1_gray,gray_canny,1,100);
    //亮度调整
    cvtColor(pic1,pic_hsv,COLOR_BGR2HSV);
    split(pic_hsv,hsv);
    hsv[2].convertTo(hsv[2],-1,1,100);
    merge(hsv,pic_hsv);
    cvtColor(pic_hsv,pic_hsv_BGR,COLOR_HSV2BGR);
    //水平/垂直反转；
    flip(pic1,pic_0,0);
    flip(pic1,pic_1,1);
    //10x10像素比对；
    while(1){
        int x,y;
        cout<<"输入想要查询的哪一块像素（输入-1 -1停止运行）"<<endl;
        cin>>x>>y;
        if(x==-1&&y==-1){
            break;
        }
        dfs(x,y);
 
    }
    save();
    cout<<"程序运行成功，请查看../save文件夹"<<endl;
}