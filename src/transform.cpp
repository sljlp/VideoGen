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
#include "tools.hpp"
#include "assert.h"

using namespace std;

#ifndef ABS
#define ABS(x) ((x)<0?-(x):(x))
#endif
#ifndef MIN_POSITIVE_FLOAT
#define MIN_POSITIVE_FLOAT 1e-12
#endif

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

#define ROTATE_AXI_X 0
#define ROTATE_AXI_Y 1
#define ROTATE_AXI_Z 2
#ifndef MATH_PI
#define MATH_PI 3.1415926
#endif
cv::Mat __Rotate(int axi, float angle) {
    cv::Mat mat(4, 4, CV_32FC1);
    mat = 0;
    cv::Mat_ <float>m = mat;
    m[3][3] = 1;
    float c = cos(angle*MATH_PI / 180);
    float s = sin(angle*MATH_PI / 180);

    switch (axi)
    {
    case ROTATE_AXI_X:
        m[0][0] = 1;
        m[1][1] = m[2][2] = c;
        m[1][2] = -s;
        m[2][1] = s;
        break;
    case ROTATE_AXI_Y:
        m[1][1] = 1;
        m[2][2] = m[0][0] = c;
        m[0][2] = s;
        m[2][0] = -s;
        break;
    case ROTATE_AXI_Z:
        m[2][2] = 1;
        m[0][0] = m[1][1] = c;
        m[0][1] = -s;
        m[1][0] = s;
        break;
    }

    return mat;

};

// input angles in degree
cv::Mat Rotate(float rx=0.0, float ry=0.0, float rz = 0.0) {
    
    return __Rotate(ROTATE_AXI_X, rx) * __Rotate(ROTATE_AXI_Y, ry) * __Rotate(ROTATE_AXI_Z, rz);
//
//    cv::Mat R(4, 4, CV_32F);
//    cv::Mat_<float> R_ (R);
//
//    R_ <<
//       1,0,0,0,
//       0,1,0,0,
//       0,0,1,0,
//       0,0,0,1;
//
//    return R;

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
    this->cacheMat = nullptr;

}

Transform::Transform(const Transform& t) {
    memcpy(this, &t, sizeof(Transform));
    if(t.cacheMat){
        this->cacheMat = new CacheTransformMat;
        assert(this->cacheMat != nullptr);
        memcpy(cacheMat, t.cacheMat, sizeof(CacheTransformMat));
        cacheMat->mat=cv::Mat();
        t.cacheMat->mat.copyTo(cacheMat->mat);
        
    }
}

Transform& Transform::operator= (const Transform& t) {
    memcpy(this, &t, sizeof(Transform));
    if(t.cacheMat){
        if(this->cacheMat){
            delete this->cacheMat;
            this->cacheMat = nullptr;
        }
        this->cacheMat = new CacheTransformMat;
        assert(this->cacheMat != nullptr);
        memcpy(cacheMat, t.cacheMat, sizeof(CacheTransformMat));
        cacheMat->mat=cv::Mat();
        t.cacheMat->mat.copyTo(cacheMat->mat);
    }
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
    
    this->cacheMat = nullptr;
}

Transform::~Transform() {
    if(cacheMat){
        delete cacheMat;
    }
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
    this->cacheMat = nullptr;
}

void Transform::processTransform(const int& imW, const int& imH, const float& centerX, const float& centerY, const int& outW, const int& outH, cv::Mat fatherMat){
    assert((fatherMat.cols == 0 || fatherMat.rows == 0) || fatherMat.type() == CV_32F);
    if (this->cacheMat){
        if ( ABS(cacheMat->centerX - centerX) < MIN_POSITIVE_FLOAT
            && ABS(cacheMat->centerY - centerY) < MIN_POSITIVE_FLOAT
            && imW == this->cacheMat->imW
            && imH == this->cacheMat->imH
//            && outH == cacheMat->outH
//            && outW == cacheMat->outW
            && (fatherMat.size() == cacheMat->father_mat.size()
                && cv::sum(cv::abs(fatherMat - cacheMat->father_mat))[0] < MIN_POSITIVE_FLOAT)
            ){
            return;
        }
    }else{
        this->cacheMat = new CacheTransformMat;
    }
    //    cv::Mat out_image(osize, image.type());
//    cv::Size osize = cv::Size(outW, outH);

    cv::Mat align_T = Translate(-centerX, -centerY);
    cv::Mat T = Translate(this->x, this->y, this->z);
    cv::Mat R = Rotate(this->rx, this->ry, this->rz);
    cv::Mat S = Scale(this->sx, this->sy, this->sz);
    cv::Mat C = T*align_T.inv()*S*R*align_T;

//    float iw = (float)imW / 2.0;
//    float ih = (float)imH / 2.0;
    float iw, ih;
    iw = this->anchorx;
    ih = this->anchory;
    cv::Mat pts_src(4,4,CV_32F);
    cv::Mat_<float> pts_src_(pts_src);
    pts_src_ <<
             centerX - iw,   centerX - iw,   centerX + iw,   centerX + iw,
             centerY - ih,   centerY + ih,   centerY - ih,   centerY + ih,
             0.0f,           0.0f,           0.0f,           0.0f,
             1.0f,           1.0f,           1.0f,           1.0f;
    //cout << C<<"\n";
    //cout << pts_src<<"\n";
    cv::Mat pts_dst = C * pts_src;
    //cout<<pts_dst<<"\n";
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

    if (fatherMat.cols != 0 && fatherMat.rows != 0){
//pts 求 inv(father_mat) as inv_father_mat
//        pts_src_vec = inv_father_mat(pts_src_vec)
//
    }
//    cv::imshow("bg", out_image);
    //cout<<"src pts"<<pts_src_vec<<"\n";
    //cout<<"src pts"<<pts_dst_vec<<"\n";
    cacheMat->mat = cv::getPerspectiveTransform(pts_src_vec, pts_dst_vec);
        
}

//该矩阵作用与输入图片，得到变换后的图像
void Transform::transformImage(const cv::Mat& image, const float& centerX, const float& centerY, cv::Mat& out_image, cv::Mat& mask) {

    assert (out_image.type() == CV_8UC3);
    assert (mask.type() == CV_8UC3);
    
    //cv::imshow("image0", image);
    
    this->processTransform(image.cols, image.rows, centerX, centerY, out_image.cols, out_image.rows);
    //cout<<cacheMat->mat<<"\n";
    //cout<<"mat:\n"<<cacheMat->mat<<"\n";
    cv::warpPerspective(image, out_image, this->cacheMat->mat, out_image.size());

    //cv::imshow("img1", out_image);
//    cv::waitKey();
    cv::Mat mask_image ;
    mask_image = cv::Mat(image.size(),mask.type(), cv::Scalar(255,255,255,255));
    //cv::imshow("m1", mask_image);
    cv::Mat mask_bg = cv::Mat(out_image.size(), mask.type(),cv::Scalar(0,0,0,0));
    cv::warpPerspective(mask_image, mask_bg, this->cacheMat->mat, mask_bg.size());
//    mask_bg = vg::erode(mask_bg);
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
void Transform::setAXYZ(float*ax, float* ay, float*az){
    if(ax){
        this->anchorx = *ax;
    }
    if(ay){
        this->anchory = *ay;
    }
    if(az){
        this->anchorz = *az;
    }
}

void Transform::followTransform(const Transform& pre_transform){
    cv::Mat mat;

}
