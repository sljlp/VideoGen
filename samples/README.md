
  
  ## genTxt.py 可以获取文字图片 （参考samples/genTxt.sh）  
genTxt.py 的参数有:  
            --font_path 字体文件路径  
            --img_size 图像尺寸，格式：h x w ，h代表图片的高，w表示宽  
            --fong_size 字体大小  
            --txt 文字内容  
            --orientation 文字排列方向 ,只能选v或者h，v表示纵向，h表示横向  
            --offset ,文字top-left位置，格式是 x y， x表示左边起始位置，y表示撒韩国那边起始位置  
            --color 格式“#+6l位16进制数字”  
            --output 文字图片输出路径  
            --mask 可选， 文字的mask输出路径  
## VideoGen可以获取最终视频(参考[samples/test.sh](https://github.com/sljlp/VideoGen/blob/master/samples/test.sh))
VideoGen 的参数有：  
            arg[1] ：json tempout fg fgm bg bgm share 6个参数作为一组输入，一次表示json文件，无声视频输出路径，前景视频，前景蒙版，背景视频，背景蒙版，分享界面图片，json是必选，其他为可选，当为可选时，必须设值为null字符串  
            arg[2]: images 若干图片路径作为输入，其中既有用户上传的图片，也有文字图片  
            arg[3]: image_mask 可选，若选择，则路径个数与iamges相等，否则，设置为null字符串  
            arg[4]: fps w h best_time， 四个参数作为一组输入 ,一次表示帧率，视频宽度，视频高度，作为分享界面的最佳时间点（单位时秒），前s三个参数一般用不到（只有没有资源视频的时候才会人为设定），可设置为0  
    调用VideoGen之后，会生成一个无声视频，接下来需要使用ffmpeg命令解析声音文件并合成到新的视频之中。


