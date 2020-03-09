
#include <iostream>

#include "resource.hpp"
#include "transform.hpp"

#include "assert.h"

#include "videoGenerator.hpp"
#include <string>
#include<vector>
using namespace std;
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<sstream>
#include "tools.hpp"

/**
 -j json_path
 -f foreground
 -m fmask
 -i images
 -n mage mask s
 
 */

#define FOUR_CODE (cv::VideoWriter::fourcc('a','v','c','1'))

int main(int argc, char** argv){
    VideoGenerator vg;
    /**
     input:
        json out fg fg_mask bg bg_mask shared
        image paths
        mask paths
        fps w h
     */
    char json_[512];
    char out_[512];
    char fg_[512], fg_mask_[512];
    char bg_[512], bg_mask_[512];
    char shared_[512];
    
    sscanf(argv[1], "%s %s %s %s %s %s %s", json_, out_, fg_, fg_mask_, bg_, bg_mask_, shared_);
    printf("json: %s\n", json_);
//    printf("")
    
    char *json=NULL;
    char *out=NULL;
    char *fg=NULL, *fg_mask=NULL;
    char *bg=NULL, *bg_mask=NULL;
    char *shared=NULL;
    
    assert(!vg::isNull(json_));json=json_;
    assert(!vg::isNull(out_));out = out_;
    if(!vg::isNull(fg_))fg = fg_;
    if(!vg::isNull(fg_mask_))fg_mask = fg_mask_;
    if(!vg::isNull(bg_))bg = bg_;
    if(!vg::isNull(bg_mask_))bg_mask = bg_mask_;
    if(!vg::isNull(shared_)) shared = shared_;
    
    vector<string> images;
    assert(!vg::isNull(argv[2]));
    stringstream iss(argv[2]);
    char img[256];
    for(;iss>>img;){
        images.push_back(img);
        printf("%s\n", img);
    }
    
    vector<string> mask(0);
    if(!vg::isNull(argv[3])){
        stringstream iss_b(argv[3]);
        for(;iss_b>>img;){
            mask.push_back(img);
        }
    }
    
    float in_fps = 0.0;
    int in_w = 0, in_h = 0;
    float best_time=0;
    
    sscanf(argv[4], "%f %d %d %f", &in_fps, &in_w, &in_h, &best_time);
    
    vg.loadData(json, &images, &mask, fg, fg_mask,bg, bg_mask);
    int fc = vg.getFrameCount();
    int w, h;
    double fps;
    if(vg.hasBackground()){
        w = vg.getWidth();
        h = vg.getHeight();
        fps = vg.getFPS();
        
    }else if(vg.hasForeground()){
        w = vg.getWidth();
        h = vg.getHeight();
        fps = vg.getFPS();
    }else{
        w = in_w;
        h = in_h;
        fps = in_fps;
    }
    
    cv::VideoWriter outV = VideoWriter(out, FOUR_CODE, fps, cv::Size(w,h), true);
    
    int best_frame_index = int(best_time * fps);
    if(best_frame_index<0)best_frame_index=0;
    if(best_frame_index>=fc)best_frame_index = fc-1;
    
    for(int i = 0;i<fc;i++){
        cv::Mat frame = vg.genFrame(i);
        outV.write(frame);
        if(i == best_frame_index && shared){
            cv::imwrite(shared, frame);
        }
    }
    outV.release();
    
}

