json/ 对json文件的操作工具包  
motion 解析 json  
transform 表示图像变换(平移，旋转，缩放，透明度等)信息  
resouce表示图像，视频等资源文件  
tools辅助工具，包含一些必要工具函数  
vedioGenerator维护motion和固定资源（background，goreground等）   
CMakeLists.txt 最后一行需要根据编译环境自主修改,现在是部分macos版本的，如果在Ubuntu下，  
    一般可以改为set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
