# VideoGen
Video according to AE  
# required
compiler supporting c++11  
ffmpeg  
opencv 4.1+ for c++  
cmake3.0+  
python3.6+  
PIL packge for python3  
opencv3+ package for python3  

### 安装ffmpeg
1.从官网下载ffmpeg安装包并解压  
2.进入安装包目录 执行 ./configure --enable-nonfree --enable-pic --enable-shared; make install  
### 安装opencv
1.从官网下载opencv安装包并解压  
2.进入解压后的安装包目录，创建build文件夹并进入build文件夹  
3. 执行 cmake -D CMAKE_CXX_FLAGS="-Wl,-Bsymbolic" ..  
4. 执行make install  

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
  在执行前看是否需要生成文字图片，在生成文字图片时需要执行py/genTxt.py
  
# 注意
#### 1.为了区分文字图层，在从AE导出json时，需要将图片图层的name（json文件中显示为关键字“nm”）的前缀设为text
