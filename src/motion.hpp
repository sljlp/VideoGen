//
//  motion.hpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef motion_hpp
#define motion_hpp

#include <stdio.h>
#include "json/json.h"

using namespace Json;

#include <string>
#include <map>
using namespace std;

#include "transform.hpp"
#include "resource.hpp"

#include <opencv2/opencv.hpp>
using namespace std;

//#define MOTINVIDEO

typedef map<string, Resource> ResMap;
typedef map<string, Transform> TransformMap;
typedef map<int, string> ImageIndexIDMap;

class Motion{
public:
    //输入json文件（该hjson文件为AE导出的json文件）
    Motion();
    Motion(const char*);
    ~Motion();
    
    //得到第frameIndex帧的第imageIndex张图片的变换
    //将变换存储到outTransform
    //若返回值为True， 则有变换，否则无变换
    bool getTransform(const int& imageIndex, const int& frameIndex, Transform&  outTransform);
    bool getTransformedImage(const int &imageIndex, const int& frameIndex, Mat &image, Mat & mask);
    Json::Value getRoot();
    int getLyaerCount();
    int getVideoW();
    int getVideoH();
    void loadResource(const char* json, const vector<string> &imagpath, const vector<string>& maskPath);
    
    int getFrameCount(){
        return this->framecount;
    }
    int getJsonStartIndex(){
        return json_start_index;
    }

private:
    Value root;
    TransformMap transformMap;
    ResMap resMap;
    ResMap maskMap;
    ImageIndexIDMap imageIndexIdMap;
    
    int video_w;
    int video_h;
    int layerCount;
    int framecount;
    int json_start_index;
};

#endif /* motion_hpp */
