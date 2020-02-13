#!/bin/sh

#  test.sh
#  VideoEffect
#
#  Created by liupeng on 2020/2/12.
#  Copyright © 2020 liupeng. All rights reserved.

# cd resouce dir, in whitch there is a std.json, std_fg.mp4, std_mask.mp4, etc. and then run this script
json=`pwd`/std.json

# produced video
out=`pwd`/temp.mp4
fg=`pwd`/std_fg.mp4;if [ ! -s $fg ];then fg=null;fi
fgm=`pwd`/std_mask.mp4;if [ ! -s $fgm ];then fgm=null;fi
bg=`pwd`/std_bg.mp4;if [ ! -s $bg ];then bg=null;fi
bgm=`pwd`/std_bg_mask.mp4;if [ ! -s $bgm ];then bgm=null;fi
share=`pwd`/shared.jpg
imgs=`find \`pwd\`/images|grep png |sort`
masks=null

# bt is the best time point (in unit of second) for shared page
# when bt=10, it means the shared page is generated at 10s of the video
$bt=10

# external music path for replacing the original music in the produced video
audio="test/wy_caishen2/audio.mp3" 

# temp video path generated by main but there i no music in it 
tempout=${out}_temp.mp4

# set main to the path to the executable binary file
$main "$json $tempout $fg $fgm $bg $bgm $share" "$imgs" "$masks" "0 0 0 $bt"

# insert audio into temp video to get the last required video
# if the $audio file exists, we use it; otherwise we use the original music contained in fg/bg
if [ -s $audio ]
then
  ffmpeg -i $audio -i $tempout $out
else
  if [ -s $fg ]
  then
    ffmpeg -i $fg ./au.mp3
    ffmpeg -i ./au.mp3 -i $tempout $out
    rm ./au.mp3
  fi
fi

rm $tempout
