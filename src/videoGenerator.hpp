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
    VideoGenerator(const char* json_path, const vector<string> *images_path, const vector<string> *mask_path, const char* foreground_path, const char* foreground_mask_path, const char* background_path = nullptr, const char* background_mask_path = nullptr);
    cv::Mat genFrame(const int& frameIndex);
    
    void loadData(const char* json_path, const vector<string> *images_path=nullptr, const vector<string> *mask_path=nullptr, const char* foreground_path=nullptr, const char* foreground_mask_path=nullptr, const char* background_path = nullptr, const char* background_mask_path = nullptr);
    int getFrameCount(){
        return motion.getFrameCount();
    }
    int getFirstImageShowIndex(){
        return motion.getJsonStartIndex();
    }
    int getWidth(){
        if(foreground.isValid() && foreground_mask.isValid()){
            return foreground.getWidth();
        }
        else if(background.isValid() && background_mask.isValid()){
            return background.getWidth();
        }
        return 0;
    }
    int getHeight(){
        if(foreground.isValid() && foreground_mask.isValid()){
            return foreground.getHeight();
        }
        else if(background.isValid() && background_mask.isValid()){
            return background.getHeight();
        }
        return 0;
    }
    
    
    bool hasBackground(){
        return background.isValid() && background_mask.isValid();
    }
    bool hasForeground(){
        return foreground_mask.isValid() && foreground.isValid();
    }
    
    double getFPS(){
        if (hasBackground()){
            return background.getFPS();
        }
        else if(hasForeground()){
            return foreground.getFPS();
        }
        return 0.0;
    }
    
};


#endif /* videoGenerater_h */
