#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ] || [ -z "$5" ] || [ -z "$6" ] || [ -z "$7" ] || [ -z "$8" ]
then
  echo "Usage: band BW channel rate pwr nvram_location time antenna"
  echo "normal value: a 40 36 8 12 /lib/modules/firmware/bcmdhd.cal 10 3"
  echo "normal value: b 20 6 7 12 /lib/modules/firmware/bcmdhd.cal 10 1"
  echo "normal value: bl 20 6 54 12 /lib/modules/firmware/bcmdhd.cal 10 0"  
  exit -1
fi

case $1 in
 "a") frq="5"
      bnd="a"
      cap="7";;
 "b") frq="2"
      bnd="b"
      cap="3";;
 "bl") frq="2"
      bnd="b"
      cap="3";;
 *) echo "wrong band"
    exit;;
esac

case $2 in
 "20") mm="2";;
 "40") mm="4";;
 *) echo "wrong BW"
    exit;;
esac

if [ -e $6 ];
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
insmod /lib/modules/3.10.0/kernel/drivers/net/wireless/bcmdhd/bcmdhd.ko firmware_path="/lib/modules/firmware/mfg_fw_4339_6_37_42_4.bin dhd_msg_level=0xffff nvram_path=$6" ; ifconfig wlan0 up

$script_dir/wl_x down
$script_dir/wl_x mpc 0 #issued this before starting other wl commands
$script_dir/wl_x phy_watchdog 0
$script_dir/wl_x country ALL
$script_dir/wl_x PM 0
$script_dir/wl_x band $bnd #note band
$script_dir/wl_x vht_features
$script_dir/wl_x bw_cap $frq $cap
$script_dir/wl_x mimo_txbw $mm #note value is 4 for 40MHz and 2 for 20MHz bandwidth
#$script_dir/wl_x chanspecs -b $frq -w $2
$script_dir/wl_x chanspec $3/$2 #note channel/bandwidth
case $1 in
 "a") $script_dir/wl_x 5g_rate -v $4;; #note MCS value
 "b") $script_dir/wl_x 2g_rate -h $4;; #note HT value
 "bl") $script_dir/wl_x 2g_rate -r $4;; #note data rate value
esac
$script_dir/wl_x up
$script_dir/wl_x phy_forcecal 1
$script_dir/wl_x phy_activecal 1
$script_dir/wl_x txpwr1 -o -d $5 #note power value

if [ $8 -gt -1 ]
then
$script_dir/wl_x txant $8
$script_dir/wl_x antdiv $8
fi

$script_dir/wl_x scansuppress 1
$script_dir/wl_x pkteng_start 00:11:22:33:44:55 tx 150 1500 0
echo "Sending for $7 seconds"
sleep $7
$script_dir/wl_x pkteng_stop tx
echo "Stop sending"

ifconfig wlan0 down > /dev/null 2>&1
rmmod bcmdhd > /dev/null 2>&1
rmmod atmel_mci > /dev/null 2>&1
#/etc/init.d/wlan-radio-pwr.script off
ltrx-radio-down
