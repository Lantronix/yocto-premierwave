#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ] || [ -z "$5" ] || [ -z "$6" ] || [ -z "$7" ] || [ -z "$8" ]
then
  echo "Usage: rtime frq lch pkt len r-mac class time"
  echo "normal value: 500 0 1 4 1000 EEFFC0880000 1 5"
  echo "normal value: 500 0 1 4 1000 EEFFC0880000 2 5"
  exit -1
fi

current_dir=$(pwd)
script_dir=$(dirname $0)

if [ $script_dir = '.' ]
then
script_dir="$current_dir"
fi

case $7 in
 "1") clf="BCM4339_003.001.009.0105.0488_ORC_USI_CL1.hcd";;
 "2") clf="BCM4339_003.001.009.0105.0489_ORC_USI_CL2.hcd";;
 *) echo "wrong BT class"
    exit;;
esac

ln -s $script_dir/mybluetool $script_dir/mybluetool_x > /dev/null 2>&1

#/etc/init.d/wlan-radio-pwr.script on
ltrx-radio-down
usleep 100000
ltrx-radio-wlan-updown up
usleep 100000
ltrx-radio-bt-updown up
usleep 100000

$script_dir/mybluetool_x -d /dev/ttyS3
echo "e" | $script_dir/mybluetool_x -d --no2bytes --tosleep=5000  --patchram $script_dir/$clf /dev/ttyS3
echo "r $6 $1 $2 4 $3 $4 $5 $8" | $script_dir/mybluetool_x -o -d /dev/ttyS3
#sleep 30
#$script_dir/mybluetool_x -d /dev/ttyS3

#/etc/init.d/wlan-radio-pwr.script off
ltrx-radio-down

#echo "c 0 0 0 0 0" |  $script_dir/mybluetool_x -o /dev/ttyS3

