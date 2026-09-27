#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
// typedef Point Poi;
bool cmp(pair<double,int>a,pair<double,int> b){
    return a.first>b.first;
}

int main(){
    // VideoCapture vi(0);
    // vi.open(0);
    VideoCapture vi("../videos/red.mp4");
    Mat mid;
    int choose=1;//red;
    //choose==1,则为红光，choose！=1是蓝光；
    //只有当摄像头之前出现了灯条之后，才能弹出来镜像框，当灯条移出去时，会导致图像画面暂停，属于正常现象；

    while(1){
        vi>>mid;
        // mid = imread("../board.jpg");
        vector<vector<Point>> a;
        //这个a是轮廓的点集合。
        Mat gray,thr,agent,mat2,ak;
        //深度拷贝，防止破坏原图。
        mid.copyTo(gray);
        mid.copyTo(mat2);
        GaussianBlur(mid,mid,Size(5,5),0);
        Mat kernel = getStructuringElement(0,Size(3,3));
        dilate(mat2,mid,kernel);
        morphologyEx(mid,mid,MORPH_OPEN,kernel);
        //高斯，腐蚀，开运算，处理光晕
        //转成hsv后，方便调掩码
        cvtColor(mid,agent,COLOR_BGR2HSV);
        if(choose==1){
            inRange(agent,Scalar(0,100,200),Scalar(10,255,255),thr);
            inRange(agent,Scalar(170,100,200),Scalar(180,255,255),ak);
            thr=thr|ak;
            inRange(agent,Scalar(15,150,150),Scalar(25,255,255),ak);
            thr=thr|ak;
        }else{
            inRange(agent,Scalar(100, 150, 200),Scalar(140,255,255),thr);
            inRange(agent,Scalar(110,180,220),Scalar(130,255,255),ak);
            thr=ak|thr;
           
        }
        
        findContours(thr,a,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        //这个是为了对轮廓的面积排序，double是面积，int是对应轮廓编号。
        vector<pair<double,int>> sort_area(a.size());
        for(int i=0;i<a.size();i++){
            RotatedRect rect(minAreaRect(a[i]));
            double pa_x,pa_y;
            pa_x=rect.size.width>rect.size.height?rect.size.width:rect.size.height;
            pa_y=rect.size.width>rect.size.height?rect.size.height:rect.size.width;
            //利用灯光条矩形的特性，筛选掉一些不符合的矩形
            if(pa_x/pa_y>8||pa_x/pa_y<2){
            sort_area[i].first=-1;
            sort_area[i].second=i;
            }
            else{sort_area[i].first=contourArea(a[i]);
            sort_area[i].second=i;}
            //55 135;
        
        }
        sort(sort_area.begin(),sort_area.end(),cmp);
        if(sort_area.size()<2){
            continue;
        }
        
        for(int j=0;j<2;j++){
            int i=sort_area[j].second;
            RotatedRect rect(minAreaRect(a[i]));
            Point2f Point1[4];
            //把点集合存入
            rect.points(Point1);
            // rectangle(mid,rect,Scalar(0,255,0),3);
            // rect.point2f(Point);

            for(int m=0;m<4;m++){
                if(m==3){
                    //当m为3时，m+1会越界，所以单独处理。
                    line(gray,Point1[m],Point1[0],Scalar(0,255,0),3);
                    break;
                }
                line(gray,Point1[m],Point1[m+1],Scalar(0,255,0),3);
            }
            //画出来单独的轮廓的中心
            circle(gray,rect.center,2,Scalar(0,255,0),2);
        }
        //获取两个灯条的中心，以此来找到装甲板的中心；
        RotatedRect rect2(minAreaRect(a[sort_area[0].second]));
        RotatedRect rect3(minAreaRect(a[sort_area[1].second]));
        line(gray,rect2.center,rect3.center,Scalar(0,255,0),2);
        circle(gray,(rect2.center+rect3.center)/2.0,2,Scalar(0,0,255),5);
        imshow("aaa",gray);
        waitKey(30);

    }
}