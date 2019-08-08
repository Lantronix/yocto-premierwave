#!/bin/bash
while true
do
    sleep 300 #5 minutes
    logger "time synced"
    echo TIME=`date +'%Y-%m-%d %H:%M:%S'` > /ltrx_private/time.store
done


