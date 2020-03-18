json/ 对json文件的操作工具包  
motion 解析 json  
transform 表示图像变换(平移，旋转，缩放，透明度等)信息  
resouce表示图像，视频等资源文件  
tools辅助工具，包含一些必要工具函数  
vedioGenerator维护motion和固定资源（background，goreground等）   
global_conf.h 编译配置文件，目前支持修改:  
VIDEO_BUFFER ON : 将视频所有帧预先加载到内存，内存使用增大，速度提高  
MATCACHE OFF : 启用变换矩阵缓存， 内存增加， 速度提高  
ENABLE_OMP OFF :开启多进程，速度增加，启用VIDEO_BUFFER，关闭MATCACHE, 内存增加,需要支持omp编程  
SERVER_MODE OFF: 程序模式 ，服务器模式/本地测试  
FOUR_CODE : 输出视频格式  
## 注意
如果opencv 编码器不支持h264，可以将main.cpp中的宏 H264_AVAILABLE 设置为FALSE， 如果支持可设置为TRUE  
或者根据具体环境自由设置宏定义 FOUR_CODE ，现在供选择的有两种 :(‘a’,‘v','c','1'), ('m','p','4','v')  
其他类型和具体用法请百度搜索  

