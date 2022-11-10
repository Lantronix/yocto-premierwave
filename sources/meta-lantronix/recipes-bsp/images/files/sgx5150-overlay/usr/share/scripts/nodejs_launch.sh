#!/bin/sh

RUN_DIR=/ltrx_user/scripts
export DIC_HOME=/usr/share/scripts

echo "Node.js version: $(/bin/node -v)" | /usr/bin/logger -t nodejs_launch.sh -p user.debug

mkdir -p $RUN_DIR
if [ ! -f $RUN_DIR/boot.js ] ; then
  cp $DIC_HOME/boot.js $RUN_DIR
fi
cd $RUN_DIR
/bin/node boot.js > /dev/null 2>&1

