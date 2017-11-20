#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]
then
  echo "Usage: band channel nvram_location time"
  echo "normal value: a 36 /lib/modules/firmware/bcmdhd.cal 10"
  echo "normal value: b 6 /script/nvram/bcmdhd.cal 10"
  exit -1
fi

case $1 in
 "a") frq="5"
      bnd="a"
      cap="7";;
 "b") frq="2"
      bnd="b"
      cap="3";;
 *) echo "wrong band"
    exit;;
esac

if [ -e $3 ];
then
  echo "Found NVram"
else
  echo "No NVram file"
  exit -1
fi

current_dir=$(pwd)
script_dir=$(dirname $0)

if [ $script_dir = '.' ]
then
script_dir="$current_dir"
fi

ln -s $script_dir/wl_7_29 $script_dir/wl_x > /dev/null 2>&1

ifconfig wlan0 down > /dev/null 2>&1
rmmod bcmdhd > /dev/null 2>&1
rmmod atmel_mci > /dev/null 2>&1

#/etc/init.d/wlan-radio-pwr.script on
ltrx-radio-down
usleep 100000
ltrx-radio-wlan-updown up
usleep 100000
insmod /lib/modules/3.10.0/kernel/drivers/mmc/host/atmel-mci.ko
insmod /lib/modules/3.10.0/kernel/drivers/net/wireless/bcmdhd/bcmdhd.ko firmware_path="/lib/modules/firmware/mfg_fw_4339_6_37_42_4.bin dhd_msg_level=0xffff nvram_path=$3" ; ifconfig wlan0 up

$script_dir/wl_x down
$script_dir/wl_x up
$script_dir/wl_x mpc 0 #issued this before starting other wl commands
$script_dir/wl_x band $bnd #note band
$script_dir/wl_x up
$script_dir/wl_x out
$script_dir/wl_x fqacurcy $2

echo "Sending for $4 seconds"
sleep $4
$script_dir/wl_x fqacurcy 0
echo "Stop sending"

ifconfig wlan0 down > /dev/null 2>&1
rmmod bcmdhd > /dev/null 2>&1
rmmod atmel_mci > /dev/null 2>&1
#/etc/init.d/wlan-radio-pwr.script off
ltrx-radio-down
