//
//  resource.hpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef resource_hpp
#define resource_hpp

#ifndef MAX
    #define MAX(x,y) ((x)>(y)?(x):(y))
    #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace cv;
#include <string>
using namespace std;


#define RES_TYPE_VIDEO 0x01
#define RES_TYPE_IMAGE 0x02

class Resource{
private:
    bool loaded;
    int res_type;
    cv::VideoCapture * cap = NULL;
    cv::Mat* image = NULL;
    
    cv::Mat nextRawImage;
    void getNextRawImage();
    
    int frameCount;
    
public:
    
    Resource(const char* );
    Resource(const string&);
    Resource();
    Resource(const Resource& res);
    
    ~Resource();
    
    Resource& operator=(const Resource& res);
    
    void load(const char*, int h = 0, int w = 0);
    Mat getNextImage(const int& frameIndex);
    
    int getType(){
        assert(loaded);
        return res_type;
    }
    
    int getFrameCount(){
        return frameCount;
    }
    
    int getWidth();
    int getHeight();
    
    bool isValid(){
        return loaded;
    }
    
    double getFPS(){
        assert(loaded && cap && image == nullptr);
        return cap->get(cv::CAP_PROP_FPS);
    }
    
};

#endif /* resource_hpp */
