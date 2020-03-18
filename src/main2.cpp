
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
#include "global_conf.h"

#if ENABLE_OMP
#include "omp.h"
#endif

/**
 -j json_path
 -f foreground
 -m fmask
 -i images
 -n mage mask s
 
 */

int main(int argc, char** argv){
#if ENABLE_OMP
    double total_t1 = omp_get_wtime();
#endif
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
#if SERVER_MODE
    sscanf(argv[1], "%s %s %s %s %s %s %s", json_, out_, fg_, fg_mask_, bg_, bg_mask_, shared_);
#else
    
//    char * root = "/Users/pengliu/Desktop/WY/VideoGen-master";
    char * root = "/lp/GenVideo/";
    sprintf(json_, "%s/samples/wy_caishen2/std.json",root);
    sprintf(out_, "%s/samples/wy_caishen2/out.mp4",root);
    sprintf(fg_, "%s/samples/wy_caishen2//std_fg.mp4", root);
    sprintf(fg_mask_, "%s/samples/wy_caishen2//std_mask.mp4",root);
    sprintf(bg_, "null");
    sprintf(bg_mask_, "null");
    sprintf(shared_, "%s/samples/wy_caishen2//shared.png", root);
#endif
    
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
#if SERVER_MODE
    assert(!vg::isNull(argv[2]));
    stringstream iss(argv[2]);
#else
    char imgs_str[2048];
    sprintf(imgs_str, "%s/samples/wy_caishen2/images//img_1.png %s/samples/wy_caishen2/images//img_0.png %s/samples/wy_caishen2/images//img_2.png", root, root
            , root);
    stringstream iss(imgs_str);
#endif
    char img[256];
    for(;iss>>img;){
        images.push_back(img);
        printf("%s\n", img);
    }
    vector<string> mask(0);
#if SERVER_MODE
    if(!vg::isNull(argv[3])){
        stringstream iss_b(argv[3]);
        for(;iss_b>>img;){
            mask.push_back(img);
        }
    }
#else
    mask.resize(0);
//    best_time = 10;
#endif
    
    float in_fps = 0.0;
    int in_w = 0, in_h = 0;
    float best_time=0;
#if SERVER_MODE
    sscanf(argv[4], "%f %d %d %f", &in_fps, &in_w, &in_h, &best_time);
#else
    best_time=10;
#endif
    
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
    
    
    int best_frame_index = int(best_time * fps);
    if(best_frame_index<0)best_frame_index=0;
    if(best_frame_index>=fc)best_frame_index = fc-1;
    std::vector<cv::Mat> frames(vg.getFrameCount());
    printf("fc: %d\n", fc);
#if ENABLE_OMP
    double t1 = omp_get_wtime();
#pragma omp parallel for
#endif
    for(int i = 0;i<fc;i++){
        cv::Mat frame = vg.genFrame(i);
        frames[i] = frame;
//        cv::imshow("img", frame);
//        cv::waitKey();
//        outV.write(frame);
    }
#if ENABLE_OMP
    double t2 = omp_get_wtime();
    printf("cost time : %.5f\n", t2 - t1);
#endif
    cv::VideoWriter outV = VideoWriter(out, FOUR_CODE, fps, cv::Size(w,h), true);
    printf("opened: %d\n", outV.isOpened());
    printf("frames count %d\n",frames.size());
    int i = 0;
    for (cv::Mat f : frames){
//        cv::imshow("img", f);
//        cv::waitKey();
//        printf("%d %d\n", f.cols, f.rows);
        outV.write(f);
//        cv::imshow("f",f);
//        cv::waitKey();
        if(i++ == best_frame_index && shared){
            cv::imwrite(shared, f);
        }
    }
    outV.release();
#if ENABLE_OMP
    double total_t2 = omp_get_wtime();
    printf("total time:%.5f\n", total_t2 - total_t1);
#endif
    printf("done\n");
}

