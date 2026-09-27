// #include <bits/stdc++.h>
// #include <opencv2/opencv.hpp>
// using namespace std;
// using namespace cv;
// vector<Mat> pre_ima={imread("../images/test1.png"),imread("../images/test2.png"),imread("../images/test3.png")};
// vector<Mat> gray_ima(3);
// vector<Mat> ima(3);
// vector<Mat> ima2(3);
// vector<Mat> imahsv(3);
// vector<Mat> imatest(3);
// vector<Mat> maked_ima(3);
// vector<vector<vector<Point>>> m(3);
// vector<vector<Point>> new_pr;
// int main(){
//     string t="../test2";
//     for(int i=0;i<3;i++){
//         cvtColor(pre_ima[i],gray_ima[i],COLOR_BGR2GRAY);
//         cvtColor(pre_ima[i],imahsv[i],COLOR_BGR2HSV);
        //这个参数非常完美，不用调；
//         inRange(imahsv[i],Scalar(80,50,45),Scalar(100,255,255),imatest[i]);//ok
//         // imwrite(t+to_string(i)+".jpg",imatest[i]);

        
//     }
//     for(int i=0;i<3;i++){
//         adaptiveThreshold(gray_ima[i],ima[i],255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);
//         // Mat k=getStructuringElement(MORPH_RECT,Size(5,5));
//         // morphologyEx(ima[i],ima2[i],MORPH_CLOSE,k);
//     }
//     string kkk="../t";
//     for(int i=0;i<3;i++){
//         maked_ima[i]=pre_ima[i].clone();
//         findContours(imatest[i],m[i],RETR_EXTERNAL,CHAIN_APPROX_NONE);
//         for(int j=0;j<m[i].size();j++){
//             vector<Point> ht; 
//             approxPolyDP(m[i][j],ht,arcLength(m[i][j],true)*0.02,true);
//             new_pr.push_back(ht);
//             double sd=contourArea(ht);
//             cout<<"area : "<<sd<<" size : "<<ht.size()<<endl;
//             if(ht.size()==4&&sd>=20){
//                 RotatedRect op_mid=minAreaRect(ht);
//                 Mat mid_2=getRotationMatrix2D(op_mid.center,op_mid.angle,1.0);
//                 Mat res;
//                 Mat rest;
//                 Rect op=boundingRect(ht);
//                 res=maked_ima[i](op);
//                 warpAffine(res,rest,mid_2,res.size());
//                 imwrite("../xx"+to_string(i)+to_string(j)+".jpg",rest);
//             }
//         }
        
        
//         drawContours(maked_ima[i],m[i],-1,Scalar(0,0,255),2);
//         imwrite(kkk+to_string(i)+".png",maked_ima[i]);
//     }
//     return 0;


    
// }



#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

vector<Mat> pre_ima = {imread("../images/test1.png"), imread("../images/test2.png"), imread("../images/test3.png")};
vector<Mat> gray_ima(3);
vector<Mat> ima(3);
vector<Mat> ima2(3);
vector<Mat> imahsv(3);
vector<Mat> imatest(3);
vector<Mat> maked_ima(3);
vector<vector<vector<Point>>> m(3);
vector<vector<Point>> new_pr;
vector<Mat> green;
vector<Mat> blue;
int main() {



    //转换green；
    for(int i=0;i < 3;i++) {
        cvtColor(pre_ima[i],gray_ima[i],COLOR_BGR2GRAY);
        cvtColor(pre_ima[i], imahsv[i],COLOR_BGR2HSV);
        inRange(imahsv[i],Scalar(80,50,45),Scalar(100,255,255),imatest[i]);
    }
    
    // for(int i = 0; i < 3; i++) {
    //     adaptiveThreshold(gray_ima[i],ima[i],255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY, 11, 2);
    // }
    
    string kkk="../out";
    for(int i=0;i<3;i++) {
        maked_ima[i]=pre_ima[i].clone();
        findContours(imatest[i],m[i],RETR_EXTERNAL,CHAIN_APPROX_NONE);
        for(int j=0;j<m[i].size();j++) {
            vector<Point> ht; 
            approxPolyDP(m[i][j],ht,arcLength(m[i][j],true)*0.02, true);
            new_pr.push_back(ht);
            double sd=contourArea(ht);
            cout << "area : "<<sd<<" size(bian) : "<<ht.size()<<endl;
            if(ht.size()==4&&sd>=20) {
                RotatedRect op_mid=minAreaRect(ht);
                double an=op_mid.angle;
                if(op_mid.size.width<op_mid.size.height) {
                    an=an+90;
                }
                Point2f cen=op_mid.center;
                Mat rota_model=getRotationMatrix2D(cen,an,1.0);
                Mat rotated_ima;
                warpAffine(maked_ima[i],rotated_ima,rota_model,maked_ima[i].size());
                //原本整形的点集合精度不够，用float类型来代替，提高旋转精度；
                vector<Point2f> points;
                for(auto& p:ht) {
                    points.push_back(Point2f(p));
                }
                vector<Point2f> rotated_points;
                transform(points,rotated_points,rota_model);
                Rect op=boundingRect(rotated_points);
                Mat res=rotated_ima(op);
                Mat rest=res.clone();
                    flip(rest,res,-1);
                green.push_back(res);
                // imwrite("../xx" + to_string(i) + to_string(j) + ".jpg", res);
                
            }
        }
        
        drawContours(maked_ima[i],m[i],-1,Scalar(0,0,255),2);
        // imwrite(kkk+to_string(i)+".png",maked_ima[i]);
    }






    //转换blue；
   
        for(int i=0;i < 3;i++) {
        cvtColor(pre_ima[i],gray_ima[i],COLOR_BGR2GRAY);
        cvtColor(pre_ima[i], imahsv[i],COLOR_BGR2HSV);
        inRange(imahsv[i],Scalar(95,50,45),Scalar(115,255,255),imatest[i]);
    }
    
    // for(int i = 0; i < 3; i++) {
    //     adaptiveThreshold(gray_ima[i],ima[i],255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY, 11, 2);
    // }
    // 38701,
    
    string kkk2="../outblue";
    for(int i=0;i<3;i++) {
        maked_ima[i]=pre_ima[i].clone();
        findContours(imatest[i],m[i],RETR_EXTERNAL,CHAIN_APPROX_NONE);
        for(int j=0;j<m[i].size();j++) {
            vector<Point> ht; 
            approxPolyDP(m[i][j],ht,arcLength(m[i][j],true)*0.02, true);
            new_pr.push_back(ht);
            double sd=contourArea(ht);
            cout <<j<<" "<<"area : "<<sd<<" size(bian) : "<<ht.size()<<endl;
            if(ht.size()==4&&sd>=30000) {
                RotatedRect op_mid=minAreaRect(ht);
                double an=op_mid.angle;
                if(op_mid.size.width<op_mid.size.height) {
                    an=an+90;
                }
                Point2f cen=op_mid.center;
                Mat rota_model=getRotationMatrix2D(cen,an,1.0);
                Mat rotated_ima;
                warpAffine(maked_ima[i],rotated_ima,rota_model,maked_ima[i].size());
                //原本整形的点集合精度不够，用float类型来代替，提高旋转精度；
                vector<Point2f> points;
                for(auto& p:ht) {
                    points.push_back(Point2f(p));
                }
                vector<Point2f> rotated_points;
                transform(points,rotated_points,rota_model);
                Rect op=boundingRect(rotated_points);
                Mat res=rotated_ima(op);
                Mat rest=res.clone();
                    // flip(rest,res,-1);
                blue.push_back(res);
                // imwrite("../xx"+to_string(i)+to_string(j)+".jpg",res);

                
            }
        }
        
        drawContours(maked_ima[i],m[i],-1,Scalar(0,0,255),2);
        // imwrite(kkk2+to_string(i)+".png",maked_ima[i]);
    }

int i=1;
for(auto it:green){
    Mat res;
    if(i==2){flip(it,res,-1);imwrite("../green/green"+to_string(i)+".jpg",res);i++;}
    else{
        imwrite("../green/green"+to_string(i)+".jpg",it);
        i++;
    }
}
i=1;
for(auto it:blue){
    Mat res;
    if(i==3){flip(it,res,-1);imwrite("../blue/blue"+to_string(i)+".jpg",res);i++;}
    else{
        imwrite("../blue/blue"+to_string(i)+".jpg",it);
        i++;
    }
}




















    return 0;
}