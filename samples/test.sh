#!/bin/sh

#  test.sh
#  VideoEffect
#
#  Created by liupeng on 2020/2/12.
#  Copyright © 2020 liupeng. All rights reserved.


json=`pwd`/std.json
out=`pwd`/temp.mp4
fg=`pwd`/std_fg.mp4;if [ ! -s $fg ];then fg=null;fi
fgm=`pwd`/std_mask.mp4;if [ ! -s $fgm ];then fgm=null;fi
bg=`pwd`/std_bg.mp4;if [ ! -s $bg ];then bg=null;fi
bgm=`pwd`/std_bg_mask.mp4;if [ ! -s $bgm ];then bgm=null;fi
share=`pwd`/shared.jpg
imgs=`find \`pwd\`/images|grep png |sort`
masks=null
$bt=0
audio="test/wy_caishen2/audio.mp3"

tempout=${out}_temp.mp4
cd Build/Products/Debug/
./VideoEffect "$json $tempout $fg $fgm $bg $bgm $share" "$imgs" "$masks" "0 0 0 $bt"

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
