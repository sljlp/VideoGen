# VideoGen

=======  
Video according to AE  

# samples usage  
#### 1.编译代码  
  mkdir build  
  cd build  
  cmake ..  
  make  
#### 2.切换目录到AE资源根文件夹(res)   
  该文件夹包含 std.json, std_fg.mp4, std_mask.mp4 等若干文件  
  cd res/ . 
#### 3.执行 samples/test.sh 脚本 
  在执行前修改脚本中的main程序路径（即第1步中编译得到的VideoGen）
