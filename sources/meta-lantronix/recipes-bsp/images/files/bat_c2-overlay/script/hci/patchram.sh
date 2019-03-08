#!/bin/sh

current_dir=$(pwd)
script_dir=$(dirname $0)

if [ $script_dir = '.' ]
then
script_dir="$current_dir"
fi

$script_dir/brcm_patchram_plus -d --patchram  $script_dir/BCM4339_003.001.009.0105.0494_ORC_USI_CL2.hcd --no2bytes --baudrate 115200 --tosleep=50000 --enable_hci /dev/ttyS3 &