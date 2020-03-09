//
//  motion.cpp
//  VideoEffect
//
//  Created by liupeng on 2020/1/8.
//  Copyright © 2020 liupeng. All rights reserved.
//

#include "motion.hpp"
#include "json/json.h"
using namespace Json;

#include <fstream>
#include <vector>
#include <string>

using namespace std;


#include "resource.hpp"
#include <stdio.h>

void __parseJson(const Value& root, const vector<string>& src_image, const vector<string>& src_image_mask_path, ResMap &resMap, ResMap &maskMap, TransformMap &transformMap, ImageIndexIDMap &imageIndexIdMap, vector<string> & layerName);
void __loadJson(const char* jsonPath, Json::Value&);

void Motion::loadResource(const char* json, const vector<string> &imagpath, const vector<string>& maskPath){
    __loadJson(json,this->root);
    __parseJson(this->root, imagpath, maskPath, this->resMap, this->maskMap, this->transformMap, this->imageIndexIdMap, this->layerName);
//    for (ResMap::iterator it = resMap.begin();it!=resMap.end();it++){
//        cv::imshow(it->first.c_str(), it->second.getNextImage(0));
//        cv::waitKey();
//    }
    
//    int video_w;
//    int video_h;
//    int layerCount;
//
    this->video_w = (int)(root["w"].asDouble());
    this->video_h = (int)(root["h"].asDouble());//视频的宽、高
    this->framecount = root["op"].asInt() - root["ip"].asInt();//视频的帧数
    this->json_start_index = root["ip"].asInt();
    
    layerCount = imageIndexIdMap.size();
    
//    Json::Value layers = root["layers"];//图层
//    for (uint i = 0; i< layers.size();i++){
//        if (layers[i]["ty"].asInt() == 2){
//            this->layerCount++;
//        }
//    }
////    this->rootStartIndex = root["ip"].asInt();
//    this->rootEndIndex = root["op"].asInt() - 1;//视频在ae中开始帧的索引和结束帧的索引
     
    
}

string parseTransformId(const int& image_id, const int& frame_id){
    char parsedId[128];
    sprintf(parsedId, "%06d%06d", image_id, frame_id);
    return string(parsedId);
}

int Motion::getVideoH(){
    return this->video_h;
}
int Motion::getVideoW(){
    return this->video_w;
}

bool Motion::getTransform(const int& imageIndex, const int& frameIndex, Transform&  outTransform){
    string tId = parseTransformId(imageIndex, frameIndex);
    if (transformMap.find(tId) != transformMap.end()){
        outTransform = transformMap[tId];
        return true;
    }
    return false;
//    Transform();
}

bool Motion::getTransformedImage(const int &imageIndex, const int& frameIndex, Mat &image, Mat & mask){
    
    Transform t;
    if(this->getTransform(imageIndex, frameIndex, t) == false){
        return false;
    };
    
    string key = this->imageIndexIdMap[imageIndex];
    Mat src_image = resMap[key].getNextImage(frameIndex);
    Mat src_mask;
    if (maskMap.find(key) != maskMap.end()){
        src_mask = maskMap[key].getNextImage(frameIndex);
        cv::imshow("image mask", src_mask);
    }else{
        assert(src_image.type() == CV_8UC3);
        src_mask = cv::Mat(src_image.size(),src_image.type(),cv::Scalar(255,255,255));
    }
    
    float cX = 0,cY = 0;
    image = cv::Mat(video_h,video_w,src_image.type());
    cv::Mat image_mask = cv::Mat(video_h,video_w,image.type());
    cv::Mat temp_mask(0,0,image.type());
    //cv::imshow("src_image", src_image);
    t.transformImage(src_image, cX, cY, image, temp_mask);
    t.transformImage(src_mask, cX, cY, image_mask, temp_mask);
//    cout<<src_image.size()<<"\n";
//    cout<<image.size()<<"\n";
//    cv::waitKey();
    
//    image.mul(temp_mask);
//    cv::Mat temp_d = cv::Mat(image.rows, image.cols, image.type(),cv::Scalar(255,255,255,255));
//    //printf("%d %d %d %d\n", temp_mask.cols, temp_mask.rows, temp_mask.type(),temp_mask.channels());
//    //printf("%d %d %d %d\n", image.cols, image.rows, image.type(),image.channels());
//    //cout<<image<<"\n"<<temp_mask<<"\n";
//    image = image.mul(temp_mask)/255;
    mask = image_mask.mul(temp_mask) / 255;
    
//    char image_mulmask[256];
//    sprintf(image_mulmask, "image_mulmask_%d",imageIndex);
//    cv::imshow(image_mulmask, image);
    
    return true;
}

Motion::Motion(const char* jsonPath) {
    Json::Reader jsonReader;
    std::ifstream ifs;
    ifs.open(jsonPath);
    jsonReader.parse(ifs, this->root);
    ifs.close();
}
Motion::Motion(){
    
}
Motion::~Motion(){
    
}


Json::Value Motion::getRoot() {
    return this->root;
}

int Motion::getLyaerCount(){
    return this->layerCount;
}



void __loadJson(const char* jsonPath, Json::Value& root){
    Json::Reader jsonReader;
    std::ifstream ifs;
    ifs.open(jsonPath);
    jsonReader.parse(ifs, root);
    ifs.close();
}

void __parseJson(const Value& root, const vector<string>& src_image, const vector<string>& src_image_mask_path, ResMap &resMap, ResMap &maskMap, TransformMap &transformMap, ImageIndexIDMap &imageIndexIdMap, vector<string>& layerName){
    
    bool ddd = 0;
//    ddd = root["ddd"].asBool();
    
    Json::Value layers = root["layers"];//图层
    Json::Value assets = root["assets"];//资源列表
    
//    ResMap resMap;
//    ResMap maskMap;
    
    /*
     "assets": [
         {
             "id": "image_0",
             "w": 450,
             "h": 700,
             "u": "images/",
             "p": "img_0.png"
         },
         {
             "id": "image_1",
             "w": 450,
             "h": 700,
             "u": "images/",
             "p": "img_1.png"
         },
         {
             "id": "image_2",
             "w": 450,
             "h": 700,
             "u": "images/",
             "p": "img_2.png"
         },
         {
             "id": "image_3",
             "w": 450,
             "h": 700,
             "u": "images/",
             "p": "img_3.png"
         }
     ],
     */
    int src_image_count = 0;
    for (int i = 0;i<assets.size();i++){
        string resId = assets[i]["id"].asString();
        if (0 ==  strcmp("image", resId.substr(0,5).c_str())){
            Resource res(src_image[src_image_count]);
//            printf("%d, %d\n", res.getHeight(), assets[i]["h"].asInt());
            assert(res.getHeight() == assets[i]["h"].asInt() && res.getWidth() == assets[i]["w"].asInt());
            resMap[resId] = res;
            if (src_image_mask_path.size() > 0){
                Resource mask(src_image_mask_path[src_image_count]);
                assert(mask.getHeight() == assets[i]["h"].asInt() && mask.getWidth() == assets[i]["w"].asInt());
                maskMap[resId] = mask;
            }
        }
        src_image_count ++;
    }
    
//    TransformMap transformMap;
    int valid_layer_count = 0;
    for (uint i = 0; i< layers.size();i++){
        if (layers[i]["ty"].asInt() == 2){ // image layer
//            assert (ddd == layers[i]["ddd"].asBool());
            layerName.push_back(layers[i]["nm"].asString());
            ddd = ddd || layers[i]["ddd"].asBool();
//            ddd = layers[i]["ddd"].asBool();
            string src_id = layers[i]["refId"].asString();
            Json::Value ks = layers[i]["ks"];
            Json::Value o = ks["o"];
            Json::Value p = ks["p"];
            Json::Value a = ks["a"];
//            cout<<a<<"\n";
            
            assert(!a.isNull());
//            if(p.isNull())
//                cout<<layers[i]<<"\n";
            assert(!p.isNull());
            
            Json::Value rx, ry, rz;
            Json::Value sx, sy, sz;
            //"ip": 181,
            //"op": 375,
            int start_frame_index = layers[i]["ip"].asInt();
            int end_frame_index = layers[i]["op"].asInt();
            
            
            
            Json::Value s = ks["s"];
            if (s.isNull()){
                s["a"] = 0;
                s["k"] = 100;
            }else{
                
            }
            
            if (!ddd){
                Json::Value r = ks["r"];
                rx["a"] = 0;
                rx["k"] = 0.0;
                ry["a"] = 0;
                ry["k"] = 0.0;
                rz = r;
//                cout<<r<<"\n";
                
            }else{
                rx = ks["rx"];
                ry = ks["ry"];
                rz = ks["rz"];
                
                if(rx.isNull()){
                    assert(ry.isNull() && rz.isNull());
                    rx["a"] = 0;
                    ry["a"] = 0;
                    rz["a"] = 0;
                    rx["k"] = 0.0;
                    ry["k"] = 0.0;
                    rz["k"] = 0.0;
                }else{
                    assert(!ry.isNull() && !rz.isNull());
                }
                
            }
            
            if (o.isNull()){
                o = Json::Value();
                o["a"] = 0;
                o["k"] = 100;
            }
            
            Transform t;
            for (int frame_index = start_frame_index;frame_index<end_frame_index;frame_index++){
                int temp_index = (uint)(frame_index - start_frame_index);
                bool last_frame = (frame_index == end_frame_index - 1);
                const char * value_key = last_frame?"e":"s";
                int value_index = last_frame?temp_index-1:temp_index;
                
                if(a["a"].asBool()){
                    t.setAX(a["k"][value_index][value_key][(uint)0].asDouble());
                    t.setAY(a["k"][value_index][value_key][(uint)1].asDouble());
                    t.setAZ(a["k"][value_index][value_key][(uint)2].asDouble());
                }else{
//                    cout<<a["k"]<<"\n";
                    t.setAX(a["k"][(uint)0].asDouble());
                    t.setAY(a["k"][(uint)1].asDouble());
                    t.setAZ(a["k"][(uint)2].asDouble());
                }
                
                if(o["a"].asBool()){
                        t.setOpacity(o["k"][value_index][value_key].asDouble()/100.0);
                }else{
                    t.setOpacity(o["k"].asDouble()/100.0);
                }
                
                if(p["a"].asBool()){
                    t.setX(p["k"][value_index][value_key][(uint)0].asDouble());
                    t.setY(p["k"][value_index][value_key][(uint)1].asDouble());
                    t.setZ(p["k"][value_index][value_key][(uint)2].asDouble());
                }else{
                    t.setX(p["k"][(uint)0].asDouble());
                    t.setY(p["k"][(uint)1].asDouble());
                    t.setZ(p["k"][(uint)2].asDouble());
                }
                
                if (rx["a"].asBool()){
                    t.setRX(rx["k"][value_index].asDouble());
                }else{
                    t.setRX(rx["k"].asDouble());
                }
                if (ry["a"].asBool()){
                    t.setRY(ry["k"][value_index].asDouble());
                }else{
                     t.setRY(ry["k"].asDouble());
                }
                if(rz["a"].asBool()){
//                    printf("-------------\n");
//                    cout<<rz["k"][value_index][value_key]<<"\n";
                    t.setRZ(rz["k"][value_index][value_key][(uint)0].asDouble());
                }else{
//                    printf("-------------------\n");
//                    cout<<rz["k"]<<"\n";
                    t.setRZ(rz["k"].asDouble());
                }
                
                if (s["a"].asBool()){
                    t.setSX(s["k"][value_index][value_key][(uint)0].asDouble()/100.0);
                    t.setSY(s["k"][value_index][value_key][(uint)1].asDouble()/100.0);
                    t.setZ(s["k"][value_index][value_key][(uint)2].asDouble()/100.0);
                    
//                    printf("%f %f %f\n", s["k"][value_index][value_key][(uint)0].asDouble()/100.0
//                           ,s["k"][value_index][value_key][(uint)1].asDouble()/100.0
//                           ,s["k"][value_index][value_key][(uint)2].asDouble()/100.0);
                }else{
                    t.setSX(s["k"][(uint)0].asDouble()/100.0);
                    t.setSY(s["k"][(uint)1].asDouble()/100.0);
                    t.setZ(s["k"][(uint)2].asDouble()/100.0);
                }
                
                Json::Value &father_layer = layers[i]["parent"];
                if (!father_layer.isNull()){
                    string f_id = parseTransformId(father_layer.asInt(), frame_index);
                    assert(transformMap.find(f_id) != transformMap.end());
                    t.followTransform(transformMap[f_id]);
                }
                
                string trans_id = parseTransformId(valid_layer_count, frame_index);
                transformMap[trans_id] = t;
                
            }
            
            imageIndexIdMap[valid_layer_count++] = src_id;
            
        }
    }
    
}

string Motion::getLayerName(int layerIndex){
    return layerName[layerIndex];
};
