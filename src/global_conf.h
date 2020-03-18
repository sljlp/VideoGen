//
//  global_conf.h
//  GenVideo
//
//  Created by liupeng on 2020/3/18.
//  Copyright © 2020 liupeng. All rights reserved.
//

#ifndef global_conf_h
#define global_conf_h
#define ON 1
#define OFF 0

/*
 *changable
 */
#define VIDEO_BUFFER ON // 将视频所有帧预先加载到内存，内存使用增大，速度提高
#define MATCACHE ON // 启用变换矩阵缓存， 内存增加， 速度提高
#define ENABLE_OMP OFF //开启多进程，速度增加，启用VIDEO_BUFFER，关闭MATCACHE, 内存增加,需要支持omp编程

#define SERVER_MODE ON // 程序模式 ，服务器模式/本地测试

//#define FOUR_CODE (cv::VideoWriter::fourcc('a','v','c','1'))
#define FOUR_CODE (cv::VideoWriter::fourcc('m','p','4','v'))


/*
 unchangable
 */
#if MATCACHE
#define ENABLE_OMP OFF
#endif

#if ENABLE_OMP
#define VIDEO_BUFFER ON
#endif

#ifndef MAX
    #define MAX(x,y) ((x)>(y)?(x):(y))
    #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#endif /* global_conf_h */
