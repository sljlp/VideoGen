//
//  resource.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "resource.hpp"
#include <assert.h>
#include <stdio.h>
#include<assert.h>

bool __RESOURCE_CPP_exists(char* filepath){
    FILE * f = fopen(filepath,"rb");
    if (f){
        fclose(f);
        return true;
    }
    return false;
}

void getReversedExtension(const char* filepath, char extension[]){
    size_t str_len = strlen(filepath);
    size_t i;
    for (i = str_len - 1;i>=0 && filepath[i] != '.';i--){
        extension[str_len - 1 - i] = filepath[i];
        //printf("i = %zu\n", i);
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
    if (frames){
        delete [] frames;
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
        
#if VIDEO_BUFFER
        assert (frames);
        nextRawImage = this->frames[nextFrameIndex];
#else
        assert(cap);
        cap->read(nextRawImage);
#endif
    }
    nextFrameIndex ++;
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
void Resource::load(const char * resPath, int w, int h){
    nextFrameIndex = 0;
    printf("loading %s ...\n", resPath);
//    assert (__RESOURCE_CPP_exists((char*)resPath));
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
//    printf("%s 666\n", reversedExtension);
    if (!strcmp("gpj", reversedExtension)
        || !strcmp("gnp", reversedExtension)
        || !strcmp("gepj", reversedExtension)){
        this->res_type = RES_TYPE_IMAGE;
        
        this->image = new cv::Mat();
//        *this->image = cv::imread(resPath);
        assert(this->image);
        cv::VideoCapture vc(resPath);
        vc.read(*(this->image));
        vc.release();
        assert(image->cols > 0 && image->rows > 0);
        
        frameCount = - 1;
        assert((image->cols == w && image -> rows == h) || (w == 0 && h == 0));
    }
    else if (0 == strcmp("4pm", reversedExtension)
             || 0 == strcmp("iva", reversedExtension)){
//        printf("is video\n");
        this->res_type = RES_TYPE_VIDEO;
        this->cap = new cv::VideoCapture(resPath);
        assert(cap);
        frameCount = cap->get(cv::CAP_PROP_FRAME_COUNT);
        int frameH = cap->get(cv::CAP_PROP_FRAME_HEIGHT);
        int frameW = cap->get(cv::CAP_PROP_FRAME_WIDTH);
//
#if VIDEO_BUFFER
        this->frames = new cv::Mat[frameCount];
        for (int t = 0; t< frameCount;t++){
            this->cap->read(this->frames[t]);
        }
        this->cap->release();
#endif
        assert(frameH == h && frameW == w || h == 0 && w == 0);
    }
    
    loaded = true;

    getNextRawImage();
}

int Resource::getWidth()const {
    if (!this->loaded)return 0;
    if(this->res_type == RES_TYPE_IMAGE)
        return this->image->cols;
    else if (res_type == RES_TYPE_VIDEO)
        return cap->get(cv::CAP_PROP_FRAME_WIDTH);
    return 0;
}
int Resource::getHeight()const {
    if(!loaded)return 0;
    if(this->res_type == RES_TYPE_IMAGE)
        return this->image->rows;
    else if(res_type == RES_TYPE_VIDEO)
        return cap->get(cv::CAP_PROP_FRAME_HEIGHT);
    return 0;
}

cv::Mat Resource::getFrameAt(const int &frameIndex){
    cv::Mat frame;
    if (this->getType() == RES_TYPE_IMAGE){
        this->image->copyTo(frame);
    }else if(this->getType() == RES_TYPE_VIDEO){
        assert (this->frames);
        assert (frameIndex >= 0 && frameIndex < this->getFrameCount());
        this->frames[frameIndex].copyTo(frame);
    }
    return frame;
}


