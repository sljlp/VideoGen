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

//static_assert(sizeof(void*)<<1 == 17,"haha");

extern void vg_blend(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat & mask1, cv::Mat mask2, OutputArray blendedimage);
extern void vg_drawOn(const cv::Mat& image, cv::Mat& bg, const cv::Mat & mask1);
//extern

cv::Mat VideoGenerator::genFrame(const int& frameIndex){
    int w = motion.getVideoW();
    int h = motion.getVideoH();
    
    cv::Mat content(h,w,CV_8UC3);
    
    cv::Mat fg = this->foreground.getNextImage(frameIndex);
    cv::Mat fg_mask = this->foreground_mask.getNextImage(frameIndex);
    
    for (int i = 0;i<this->motion.getLyaerCount();i++){
        
        cv::Mat image, mask;
        bool hast = motion.getTransformedImage(i, frameIndex, image, mask);
        if (hast){
            vg_drawOn(image, content, mask);
        }
    }
    
    cv::Mat content_mask(h,w,CV_8UC3,cv::Scalar(255,255,255));
    cv::Mat frame;
//    frame.copyTo(image2);
    vg_blend(fg, content, fg_mask, content_mask, frame);
    return frame;
    
    
};

VideoGenerator::VideoGenerator(){
    
};

VideoGenerator::VideoGenerator(const char* json_path, const char* foreground_path, const char* foreground_mask_path, const vector<string> *images_path, const vector<string> *mask_path, const char* audio_path, const char* background_path, const char* background_mask_path){
    loadData(json_path, foreground_path, foreground_mask_path, images_path, mask_path);
    
};

void VideoGenerator::loadData(const char* json_path, const char* foreground_path, const char* foreground_mask_path, const vector<string> *images_path, const vector<string> *mask_path, const char* audio_path, const char* background_path, const char* background_mask_path){
    this->image_paths = *images_path;
    if(mask_path){
        this->mask_paths = *mask_path;
    }
    assert(mask_paths.size() == 0 || mask_paths.size() == image_paths.size());
    
    motion.loadJson(json_path);
    foreground.load(foreground_path);
    foreground_mask.load(foreground_mask_path);
    if (background_path)
        background.load(background_path);
    if(background_mask_path)
        background_mask.load(background_mask_path);
    
    if (audio_path){
        this->audio_path = audio_path;
    }
    
}
