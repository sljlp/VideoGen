//
//  tools.hpp
//  VideoEffect
//
//  Created by liupeng on 2020/2/12.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef tools_hpp
#define tools_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
namespace vg{
    void vg_blend(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat & mask1, cv::Mat mask2, OutputArray blendedimage);

    void vg_drawOn(const cv::Mat& image, cv::Mat& bg, const cv::Mat & mask1);

    bool isNull(char* str);

    bool isText(char* str);

    cv::Mat erode(cv::Mat mask, int ksize=2);
}

//dst = cv.erode(binary,kernel)

#endif /* tools_hpp */
