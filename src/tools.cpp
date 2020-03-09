//
//  tools.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/2/12.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "tools.hpp"
#ifndef MAX
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

void vg::vg_blend(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat & mask1, cv::Mat mask2, OutputArray blendedimage){
    assert (image1.type() == CV_8UC3 );
    assert(image2.type() == image1.type());
    assert (mask1.type() == image1.type());
    
//    printf("%d %d %d\n ", CV_8U, blendedimage.type(), image1.type());
    assert(blendedimage.type() == image1.type());
    assert(image1.size() == image2.size());
    assert(mask1.size() == mask2.size());
    assert(image1.size() == mask1.size());
    
    cv::Mat out = image1.mul(mask1) / 255 + image2.mul(mask2) / 255;
    out.copyTo(blendedimage);
    
}

void vg::vg_drawOn(const cv::Mat& image, cv::Mat& bg, const cv::Mat & mask1){
    assert (image.type() == CV_8UC3 );
    
    cv::copyTo(image, bg, mask1);
//    out.copyTo(blendedimage);
}

bool vg::isNull(char* str){
    return strcmp(str, "NULL") == 0 || strcmp(str, "null") == 0;
}
bool vg::isText(char* str){
    if (strlen(str)<4){
        return false;
    }
    char cache[5];
    memcpy(cache, str, sizeof(char) * MIN(strlen(str),5));
    cache[4] = '\0';
    return strcmp("text",cache) == 0;
}

cv::Mat vg::erode(cv::Mat mask, int ksize){
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ksize,ksize));
    cv::Mat eroded_mask, image;
    mask.copyTo(image);
    cv::erode(image,eroded_mask,kernel);
    return eroded_mask;
//    erode( InputArray src, OutputArray dst, InputArray kernel,
}
