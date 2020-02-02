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
    bool ddd;
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
    
    //先变pre_transform 然后再进行thistransform， z最后返回结果
    Transform followTransform(const Transform& pre_transform);
    
    //该矩阵作用与输入图片，得到变换后的图像
    void transformImage(const cv::Mat& image,const float& centerX, const float& centerY, cv::OutputArray out_image, cv::OutputArray mask);
    
    float getOpacity();
    
    void setXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setSXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setRXYZ(float* x = nullptr, float* y = nullptr, float* z = nullptr);
    void setOpacity(float);
    void setDDD(bool);
    
    inline void setX(float x){
        setXYZ(&x, 0, 0);
    }
    inline void setY(float y){
        setXYZ(0,&y,0);
    }
    inline void setZ(float z){
        setXYZ(0,0,&z);
    }
    inline void setRX(float rx){
        setRXYZ(&rx,0,0);
    }
    inline void setRY(float ry){
        setRXYZ(0,&ry,0);
    }
    inline void setRZ(float rz){
        setRXYZ(0,0,&rz);
    }
    inline void setSX(float sx){
        setSXYZ(&sx,0,0);
    }
    inline void setSY(float sy){
        setSXYZ(0,&sy,0);
    }
    inline void setSZ(float sz){
        setSXYZ(0,0,&sz);
    }
    
    
    
    
};

#endif /* transform_hpp */
