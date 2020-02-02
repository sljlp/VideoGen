//
//  transform.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/16.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "transform.hpp"
#include "math.h"
#include <vector>

#include "assert.h"

using namespace std;

//Mat Transform::getMatrix(float iw, float ih){
//    float x = this->x,y = this->y,z = this->z;
//    float rx,ry,rz;
//    float sx,sy,sz;
//    if (!ddd){
//
//    }
//}

cv::Mat Translate(float x,float y,float z = 0.0) {
    cv::Mat T(4, 4, CV_32F);
    cv::Mat_<float> T_(T);
//    T_[0][3] = x;
//    T_[1][3] = y;
//    T_[2][3] = z;

    T_ <<
       1,0,0,x,
       0,1,0,y,
       0,0,1,z,
       0,0,0,1;

    return T;
}

cv::Mat Rotate(float rx, float ry, float rz = 0.0) {
    cv::Mat R(4, 4, CV_32F);
    cv::Mat_<float> R_ (R);

    R_ <<
       1,0,0,0,
       0,1,0,0,
       0,0,1,0,
       0,0,0,1;

    return R;

}

cv::Mat Scale(float sx, float sy, float sz) {
    cv::Mat S(4, 4, CV_32F);
    cv::Mat_<float> S_(S);
    S_ <<
       sx,0, 0, 0,
       0, sy,0, 0,
       0, 0, sz,0,
       0, 0, 0, 1;
    return S;
}

Transform::Transform(float x,float y,float z, float rx, float ry, float rz, float sx,float sy, float sz, float opcity, bool ddd) {
    this->x = x;
    this->y = y;
    this->z = z;

    this->rx = rx;
    this->ry = ry;
    this->rz = rz;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->opcity = opcity;
    this->ddd = ddd;

}

Transform::Transform(const Transform& t) {
    this->x = t.x;
    this->y = t.y;
    this->z = t.z;

    this->rx = t.rx;
    this->ry = t.ry;
    this->rz = t.rz;

    this->sx = t.sx;
    this->sy = t.sy;
    this->sz = t.sz;

    this->opcity = t.opcity;
    this->ddd = t.ddd;
}

Transform& Transform::operator= (const Transform& t) {
    this->x = t.x;
    this->y = t.y;
    this->z = t.z;

    this->rx = t.rx;
    this->ry = t.ry;
    this->rz = t.rz;

    this->sx = t.sx;
    this->sy = t.sy;
    this->sz = t.sz;

    this->opcity = t.opcity;
    this->ddd = t.ddd;
    return *this;
}
Transform::Transform() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;

    this->rx = 0.0;
    this->ry = 0.0;
    this->rz = 0.0;

    this->sx = 1.0;
    this->sy = 1.0;
    this->sz = 1.0;

    this->opcity = 1.0;
    this->ddd = 0;
}

Transform::~Transform() {

}

void Transform::set(float x,float y,float z, float rx, float ry, float rz, float sx,float sy, float sz, float opcity, bool ddd) {
    this->x = x;
    this->y = y;
    this->z = z;

    this->rx = rx;
    this->ry = ry;
    this->rz = rz;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->opcity = opcity;
    this->ddd = ddd;
}

//该矩阵作用与输入图片，得到变换后的图像
void Transform::transformImage(const cv::Mat& image, const float& centerX, const float& centerY, cv::OutputArray out_image, cv::OutputArray mask) {

    assert (out_image.type() == CV_8UC3);
    assert (mask.type() == CV_32F || mask.type() == CV_8U || mask.type() == CV_8UC3);
//    assert ()

    cout << out_image.size()<<"\n";

//    cv::Mat out_image(osize, image.type());
    cv::Size osize = out_image.size();

    cv::Mat align_T = Translate(-centerX, -centerY);
    cv::Mat T = Translate(this->x, this->y, this->z);
    cv::Mat R = Rotate(this->rx, this->ry, this->rz);
    cv::Mat S = Scale(this->sx, this->sy, this->sz);
    cv::Mat C = T*align_T.inv()*S*R*align_T;

    float iw = (float)image.cols / 2.0;
    float ih = (float)image.rows / 2.0;
    cv::Mat pts_src(4,4,CV_32F);
    cv::Mat_<float> pts_src_(pts_src);
    pts_src_ <<
             centerX - iw,   centerX - iw,   centerX + iw,   centerX + iw,
             centerY - ih,   centerY + ih,   centerY - ih,   centerY + ih,
             0.0f,           0.0f,           0.0f,           0.0f,
             1.0f,           1.0f,           1.0f,           1.0f;

    cv::Mat pts_dst = C * pts_src;
    cv::Mat pts_mid(4,4,CV_32F);
    pts_src.copyTo(pts_mid);
    cv::Mat_<float> pts_mid_(pts_mid);
    for (int i = 0; i<4; i++) {
        pts_mid_[0][i] -= centerX - iw;
        pts_mid_[1][i] -= centerY - ih;
    }

    vector<cv::Point2f > pts_src_vec(4);
    vector<cv::Point2f > pts_dst_vec(4);

    for (int i = 0; i<4; i++) {
        pts_src_vec[i] = cv::Point2f(pts_mid_[0][i],pts_mid_[1][i]);
//        pts_src_vec[i][0] = ;
//        pts_src_vec[i][1] = ;
        pts_dst_vec[i]  = cv::Point2f(pts_dst.at<float>(0,i),pts_dst.at<float>(1,i));
//        pts_dst_vec[i][0] = pts_dst.at<float>(0,i);
//        pts_dst_vec[i][1] = pts_dst.at<float>(1,i);
    }

    cv::Mat perspect_mat = cv::getPerspectiveTransform(pts_src_vec, pts_dst_vec);
//    cv::imshow("bg", out_image);

    cv::warpPerspective(image, out_image, perspect_mat, osize);

    cv::Mat mask_image ;
    if (CV_32F == mask.type()) {
        mask_image = cv::Mat::ones(image.size(), mask.type());
    } else {
        mask_image = cv::Mat(image.size(),mask.type(), cv::Scalar(255,255,255));
    }
    cv::imshow("m1", mask_image);
    cv::Mat mask_bg = cv::Mat::zeros(out_image.size(), mask.type());
    cv::warpPerspective(mask_image, mask_bg, perspect_mat, mask_bg.size());
    if (mask_bg.type() != CV_32F) {
        mask_bg *=  255;
    }
    mask_bg.copyTo(mask);

}
float Transform::getOpacity(){
    return opcity;
};

void Transform::setXYZ(float* x, float* y, float* z){
    if(x){
        this->x = *x;
    }
    if(y){
        this->y = *y;
    }
    if(z){
        this->z = *z;
    }
};
void Transform::setSXYZ(float* x , float* y, float* z){
    if(x){
        this->sx = *x;
    }
    if(y){
        this->sy = *y;
    }
    if(z){
        this->sz = *z;
    }
};
void Transform::setRXYZ(float* x, float* y, float* z){
    if(x){
        this->rx = *x;
    }
    if(y){
        this->ry = *y;
    }
    if(z){
        this->rz = *z;
    }
};
void Transform::setOpacity(float op){
    this->opcity = op;
};
void Transform::setDDD(bool ddd){
    this->ddd = ddd;
};
