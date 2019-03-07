#! /bin/bash

if [ -z "$1" ]; then
    $0 -h
    exit
fi

if [ "$1" == "--help" ] || [ "$1" == "-h" ] ; then
    echo -e "Supporting targets:"
    echo -e "pw2050 (Run \"./customer_set_target.sh pw2050\")"
    echo -e "sgx5150 (Run \"./customer_set_target.sh sgx5150\")"
    echo -e "bat-c2 (Run \"./customer_set_target.sh bat-c2\")"
    exit
fi

prod_name=`echo "$1" | awk '{print tolower($0)}'`
if [ "$prod_name" != "pw2050" ] && [ "$prod_name" != "sgx5150" ] && [ "$prod_name" != "bat-c2" ]; then
    echo "$1 is not a valid target."
    $0 -h
    exit
fi

if [ "$prod_name" = "sgx5150" ]; then
    #Updating target machine
    sed -i '/MACHINE =/c\MACHINE = "sgx5150"' build/conf/local.conf
    cp sources/meta-lantronix/conf/sgx5150_layer.conf sources/meta-lantronix/conf/layer.conf
elif [ "$prod_name" = "pw2050" ]; then
    #Updating target machine
    sed -i '/MACHINE =/c\MACHINE = "pw2050"' build/conf/local.conf
    cp sources/meta-lantronix/conf/pw2050_layer.conf sources/meta-lantronix/conf/layer.conf
elif [ "$prod_name" = "bat-c2" ]; then
    #Updating target machine
    sed -i '/MACHINE =/c\MACHINE = "bat-c2"' build/conf/local.conf
    cp sources/meta-lantronix/conf/bat-c2_layer.conf sources/meta-lantronix/conf/layer.conf
fi

ltrx_linux_inc=sources/meta-lantronix/recipes-kernel/linux/ltrx-linux.inc
echo "SRC_URI += \"file://linux-at91.tar.xz \"" > $ltrx_linux_inc

