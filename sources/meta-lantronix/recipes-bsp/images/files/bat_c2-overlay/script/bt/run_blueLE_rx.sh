#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ]
then
  echo "Usage: ch time"
  echo "normal value: 0 10"
  exit -1
fi

current_dir=$(pwd)
script_dir=$(dirname $0)

if [ $script_dir = '.' ]
then
script_dir="$current_dir"
fi

ln -s $script_dir/mybluetool $script_dir/mybluetool_x > /dev/null 2>&1

#/etc/init.d/wlan-radio-pwr.script on
ltrx-radio-down
usleep 100000
ltrx-radio-wlan-updown up
usleep 100000
ltrx-radio-bt-updown up
usleep 100000

$script_dir/mybluetool_x -d /dev/ttyS3
echo "e" | $script_dir/mybluetool_x -d --no2bytes --tosleep=5000  --patchram $script_dir/BCM4339_003.001.009.0105.0488_ORC_USI_CL1.hcd /dev/ttyS3
echo "q $1 $2" | $script_dir/mybluetool_x -o -d /dev/ttyS3
$script_dir/mybluetool_x -d /dev/ttyS3

#/etc/init.d/wlan-radio-pwr.script off
ltrx-radio-down

#echo "c 0 0 0 0 0" |  $script_dir/mybluetool_x -o /dev/ttyS3

