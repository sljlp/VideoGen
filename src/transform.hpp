//
//  transform.hpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/16.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp


#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace cv;

class Transform{
private:
    float x, y, z;
    float rx, ry, rz;
    float sx, sy, sz;
    float opcity;
    float anchorx,anchory,anchorz;
    bool ddd;
    struct CacheTransformMat{
        int imW, imH;
        int outW, outH;
        float centerX, centerY;
        cv::Mat father_mat;
        cv::Mat mat;
    }* cacheMat = NULL;
    
public:
    
    //x, y, z, rx, ry, rz,sx,sy,sz, o, ddd
    Transform(float, float, float, float, float, float,float, float, float, float, bool);
    Transform(const Transform& t);
    Transform();
    ~Transform();
    
    Transform& operator= (const Transform& t);
    
    
    void set(float, float, float, float, float, float,float, float, float, float, bool);
    //获得当前变换的matrix表示
    Mat getMatrix();
    
    //if there is a cache return true ,otherwise return false
    bool getCacheMat(cv::Mat& mat){
        if(cacheMat == nullptr){
            return false;
        }
        this->cacheMat->mat.copyTo(mat);
        return true;
    };
    //先变pre_transform 然后再进行thistransform， z最后返回结果
    void followTransform(const Transform& pre_transform);
    
    void processTransform(const int&, const int&, const float&, const float&, const int& outW = 0, const int& outH = 0, cv::Mat fatherMat = cv::Mat());
    //该矩阵作用与输入图片，得到变换后的图像
    void transformImage(const cv::Mat& image,const float& centerX, const float& centerY, cv::Mat& out_image, cv::Mat& mask);
    
    float getOpacity();
    
    void setXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setSXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setRXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setOpacity(float);
    void setDDD(bool);
    
    inline void setX(float x){
        this->x = x;
    }
    inline void setY(float y){
        this->y = y;
    }
    inline void setZ(float z){
        this->z = z;
    }
    inline void setRX(float rx){
        this->rx = rx;
    }
    inline void setRY(float ry){
        this->ry = ry;
    }
    inline void setRZ(float rz){
        this->rz = rz;
    }
    inline void setSX(float sx){
        this->sx = sx;
    }
    inline void setSY(float sy){
        this->sy = sy;
    }
    inline void setSZ(float sz){
        this->sz = sz;
    }
    
    void setAXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    inline void setAX(float x){
        this->anchorx = x;
    }
    inline void setAY(float y){
        anchory = y;
    }
    inline void setAZ(float z){
//        setAXYZ(nullptr,nullptr,&z);
        anchorz = z;
    }
    
};

#endif /* transform_hpp */
