#!/bin/sh


if [ "$1" == "start" ]; then
	sleep 10
	bluetool -d --no2bytes --tosleep=5000 --patchram /script/bt/BCM4339_003.001.009.0105.0488_ORC_USI_CL1.hcd --baudrate 115200 --enable_hci /dev/ttyS3 &
	#sleep 20
	#hciconfig hci0 up
fi	
