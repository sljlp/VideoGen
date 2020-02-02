//
//  videoGenerater.h
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef videoGenerator_h
#define videoGenerator_h
#include "json/json.h"
using namespace Json;

#include <string>
#include<vector>
using namespace std;

#include "motion.hpp"

class VideoGenerator{
private:
    string resource_path;
    Motion motion;
    Resource foreground, foreground_mask, background, background_mask;
    string audio_path;
    vector<string> image_paths;
    vector<string> mask_paths;
public:
    VideoGenerator();
    VideoGenerator(const char* json_path, const char* foreground_path, const char* foreground_mask_path, const vector<string> *images_path, const vector<string> *mask_path, const char* audio_path = nullptr, const char* background_path = nullptr, const char* background_mask_path = nullptr);
    cv::Mat genFrame(const int& frameIndex);
    
    void loadData(const char* json_path, const char* foreground_path, const char* foreground_mask_path, const vector<string> *images_path, const vector<string> *mask_path, const char* audio_path = nullptr, const char* background_path = nullptr, const char* background_mask_path = nullptr);
    
    
};


#endif /* videoGenerater_h */
