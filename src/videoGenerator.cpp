//
//  videoGenerator.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "videoGenerator.hpp"
#include <opencv2/opencv.hpp>
#include "assert.h"
#include "tools.hpp"

cv::Mat VideoGenerator::genFrame(const int& frameIndex){
    int w = motion.getVideoW();
    int h = motion.getVideoH();
    
    cv::Mat content(h,w,CV_8UC3,cv::Scalar(0,0,0,0));
    
    vector<int> txtLayerIndex(0);
    for (int i = 0;i<this->motion.getLyaerCount();i++){
        if(vg::isText((char*)motion.getLayerName(i).c_str())){
            txtLayerIndex.push_back(i);
        }
        cv::Mat image, mask(0,0,CV_8UC3);
        bool hast = motion.getTransformedImage(i, frameIndex, image, mask);
        //printf("has transformation:%d\n", hast);
        if (hast){
            //cv::imshow("image1", image);
            vg::vg_drawOn(image, content, mask);
        }
    }
    
    cv::Mat content_mask(h,w,CV_8UC3,cv::Scalar(255,255,255, 255));
    cv::Mat frame(h,w,CV_8UC3, cv::Scalar(0,0,0,0));
    content.copyTo(frame);
    if(foreground.isValid() && foreground_mask.isValid()){
        cv::Mat fg = this->foreground.getNextImage(frameIndex);
        cv::Mat fg_mask = this->foreground_mask.getNextImage(frameIndex);
        content_mask = cv::Scalar(255,255,255,255) - fg_mask;
        cv::Mat temp_fg_mask = cv::Mat(fg_mask.size(),CV_8UC3,cv::Scalar(255,255,255,255));
        vg::vg_blend(fg, content, temp_fg_mask, content_mask, frame);
    }
    if(background.isValid() && background_mask.isValid()){
        frame.copyTo(content);
        cv::Mat bg = this->background.getNextImage(frameIndex);
        cv::Mat bg_mask = this->background_mask.getNextImage(frameIndex);
        content_mask = cv::Scalar(255,255,255,255) - bg_mask;
        cv::Mat temp_bg_mask = cv::Mat(bg_mask.size(),CV_8UC3,cv::Scalar(255,255,255,255));
        vg::vg_blend(bg, content, temp_bg_mask, content_mask, frame);
    }
    
    for (int i : txtLayerIndex){
//        if(isText((char*)motion.getLayerName(i).c_str())){
//            txtLayerIndex.push_back(i);
//        }
        cv::Mat image, mask(0,0,CV_8UC3);
        bool hast = motion.getTransformedImage(i, frameIndex, image, mask);
        //printf("has transformation:%d\n", hast);
        if (hast){
            //cv::imshow("image1", image);
            //cv::imshow("mask", mask);
            vg::vg_drawOn(image, frame, mask);
//            cv::waitKey();
        }
    }
    

    return frame;
    
};

VideoGenerator::VideoGenerator(){
    
};

VideoGenerator::VideoGenerator(const char* json_path, const vector<string> *images_path, const vector<string> *mask_path, const char* foreground_path, const char* foreground_mask_path, const char* background_path, const char* background_mask_path){
    loadData(json_path, images_path, mask_path, foreground_path, foreground_mask_path, background_path ,background_mask_path);
    
};

void VideoGenerator::loadData(const char* json_path, const vector<string> *images_path, const vector<string> *mask_path, const char* foreground_path, const char* foreground_mask_path, const char* background_path, const char* background_mask_path){
    assert(images_path);
        this->image_paths = *images_path;
    if(mask_path){
        this->mask_paths = *mask_path;
    }
    else{
        this->mask_paths.resize(0);
    }
    assert(mask_paths.size() == 0 || mask_paths.size() == image_paths.size());
    
    motion.loadResource(json_path, this->image_paths, this->mask_paths);
    if (foreground_path){
        foreground.load(foreground_path);
    }
    if (foreground_mask_path){
        foreground_mask.load(foreground_mask_path);
    }
    
    if(foreground.isValid() && foreground_mask.isValid()){
        assert(this->getFrameCount() <= this->foreground.getFrameCount());
        assert(this->getFrameCount() <= this->foreground_mask.getFrameCount());
    }
    
    if (background_path){
        background.load(background_path);
        if(foreground_mask.isValid() && foreground.isValid()){
            assert(getFrameCount() == background.getFrameCount());
        }
    }
    if(background_mask_path){
        background_mask.load(background_mask_path);
        if(foreground.isValid() && foreground_mask.isValid()){
            assert(foreground_mask.getFrameCount() == background_mask.getFrameCount());
        }
    }
    
    if(background.isValid() && background_mask.isValid()){
        assert(this->getFrameCount() <= this->background.getFrameCount());
        assert(this->getFrameCount() <= this->background_mask.getFrameCount());
    }
}
