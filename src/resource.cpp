//
//  resource.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "resource.hpp"
#include <assert.h>

void getReversedExtension(const char* filepath, char extension[]){
    size_t str_len = strlen(filepath);
    size_t i;
    for (i = str_len - 1;i>=0 && filepath[i] != '.';i--){
        extension[str_len - 1 - i] = filepath[i];
        printf("i = %zu\n", i);
    }
    extension[str_len - 1 - i] = '\0';
    assert(filepath[i] == '.');
    
}

Resource::Resource(const char* resPath){
    load(resPath);
}

Resource::Resource(const string& resPath){
    load(resPath.c_str());
}
Resource::Resource(const Resource& res){
    loaded = res.loaded;
    res_type = res.res_type;
    cv::VideoCapture * cap = NULL;
    if (res.cap){
        cap = new cv::VideoCapture(*res.cap);
    }
    if (res.image){
        image = new cv::Mat(*res.image);
    }
    res.nextRawImage.copyTo(nextRawImage);
    frameCount = res.frameCount;
};

Resource& Resource::operator=(const Resource& res){
    loaded = res.loaded;
    res_type = res.res_type;
    cv::VideoCapture * cap = NULL;
    if (res.cap){
        cap = new cv::VideoCapture(*res.cap);
    }
    if (res.image){
        image = new cv::Mat(*res.image);
    }
    res.nextRawImage.copyTo(nextRawImage);
    frameCount = res.frameCount;
    return *this;
}

Resource::~Resource(){
    if (image){
        delete image;
    }
    if (cap){
        cap->release();
        delete cap;
    }
}

cv::Mat Resource::getNextImage(const int &frameIndex){
    assert(loaded);
    cv::Mat nextImage(nextRawImage.rows, nextRawImage.cols, nextRawImage.type());
    nextRawImage.copyTo(nextImage);
    getNextRawImage();
    return nextImage;
}

void Resource::getNextRawImage(){
    if (res_type == RES_TYPE_IMAGE){
        nextRawImage = *(this->image);
    }
    else if (res_type == RES_TYPE_VIDEO){
        cap->read(nextRawImage);
    }
}

Resource::Resource(){
    if(this->image){
        delete image;
        this->image = NULL;
    }
    if(cap){
        delete cap;
        cap = NULL;
    }
    
    loaded = false;
    
}


// 目前支持  image: jpg/jpeg, png, video: mp4, avi
void Resource::load(const char * resPath){
    
    if (cap){
        cap->release();
        delete cap;
        cap = NULL;
    }
    if(image){
        delete image;
        image = NULL;
    }
    
    char reversedExtension[10];
    getReversedExtension(resPath, reversedExtension);
    printf("%s 666\n", reversedExtension);
    if (!strcmp("gpj", reversedExtension)
        || !strcmp("gnp", reversedExtension)
        || !strcmp("gepj", reversedExtension)){
        this->res_type = RES_TYPE_IMAGE;
        this->image = new cv::Mat();
        *(this->image) = cv::imread(resPath);
        frameCount = - 1;
    }
    else if (0 == strcmp("4pm", reversedExtension)
             || 0 == strcmp("iva", reversedExtension)){
        printf("is video\n");
        this->res_type = RES_TYPE_VIDEO;
        this->cap = new cv::VideoCapture(resPath);
        frameCount = cap->get(cv::CAP_PROP_FRAME_COUNT);
    }
    
    loaded = true;

    getNextRawImage();
}

int Resource::getWidth(){
    if (!this->loaded)return 0;
    if(this->res_type == RES_TYPE_IMAGE)
        return this->image->cols;
    else if (res_type == RES_TYPE_VIDEO)
        return cap->get(cv::CAP_PROP_FRAME_WIDTH);
    return 0;
}
int Resource::getHeight(){
    if(!loaded)return 0;
    if(this->res_type == RES_TYPE_IMAGE)
        return this->image->rows;
    else if(res_type == RES_TYPE_VIDEO)
        return cap->get(cv::CAP_PROP_FRAME_HEIGHT);
    return 0;
}


