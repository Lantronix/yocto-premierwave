#!/bin/sh

###########################################################
#
# p2p_interface_ctrl.sh:
#   Controls the P2P network interface created by the
#   WPA supplicant/WLAN driver.
#
# $1: interface name
# $2: role (GO/client)
# $3: ADD/REMOVE interface
#     
#
###########################################################


DHCP_SERVER_CONFIG_FILE=/etc/dnsmasq.conf

wait_us_fileexists() {
    wait_us=`expr $1`
    wait_interval_us=100000

    while [ $wait_us -gt 0 ]
        do
            [ -e $2 ] && return 0;
            usleep $wait_interval_us
            wait_us=`expr $wait_us - $wait_interval_us`
        done
    return 1
}

# For the time being we're using dnsmasq...
update_dhcp_server_config() {

    if [ -e "$DHCP_SERVER_CONFIG_FILE" ]; then
        /bin/rm -f "$DHCP_SERVER_CONFIG_FILE"
    fi
    sleep 1

    echo "no-resolv" >> "$DHCP_SERVER_CONFIG_FILE"
    echo "interface=$1" >> "$DHCP_SERVER_CONFIG_FILE"
    echo "dhcp-range=192.168.2.2,192.168.2.20,255.255.255.0,12h" >> "$DHCP_SERVER_CONFIG_FILE"
    sync
    sleep 1
    return 1
}

# We want to have a high metric for the P2P interface in case
# the WLAN0 interface is put on a network with the same subnet
# settings. We want WLAN0 to have higher precidence.
update_p2p_route_metric() {
    p2pnet=`route | grep "$1" | awk -F ' '  '{print $1}'`
    p2pmask=`route | grep "$1" | awk -F ' '  '{print $3}'`

    if [ -n "$p2pnet" -a -n "$p2pmask" ] ; then
        logger "Changing metric for $1 net = $p2pnet mask = $p2pmask"
        route del -net $p2pnet netmask $p2pmask dev $1
        sleep 1
        route add -net $p2pnet netmask $p2pmask dev $1 metric 100
    else
        logger "Failed to update P2P route metric."
        return 0
    fi

    return 1
}
kill_process() {
    PROC_NAME=$1
    PID_FILE=$2

    if [ ! -r $PID_FILE ]; then
       logger "Unable to locate $1 PID."
       return
    fi

    PID=`cat $PID_FILE`
    if [ $PID -gt 0 ]; then
       if ps $PID | grep -q $PROC_NAME; then
          kill $PID
	    fi
    fi
    rm $PID_FILE
}


if [ "$#" -lt 3 ] ; then
        logger "P2P startup: Invalid parameters."
        exit 1
fi

if [ "$3" == "ADD" ]; then

    wait_us_fileexists 500000 /sys/class/net/"$1"
    if [ "$?" -ne "0" ] ; then
        logger "Interface $1 does not exist"
        return 1
    fi

    if [ "$2" == "GO" ] ; then
       logger "P2P GO mode."
       /sbin/ifconfig "$1" 192.168.2.1 netmask 255.255.255.0 up
        sleep 2
        #update_p2p_route_metric $1
        update_dhcp_server_config $1
        /usr/sbin/dnsmasq -x /tmp/dnsmasq-$1.pid
    elif [ "$2" == "client" ]; then
        logger "P2P client mode."
        udhcpc -i "$1" -p /tmp/udhcpc-$1.pid
    else
        logger "Invalid role, must be GO or client"
        exit 1
    fi
elif [ "$3" == "REMOVE" ]; then
    if [ ! -e /sys/class/net/"$1" ]; then
        logger "Interface $1 does not exist."
        exit 1
    fi
    if [ "$2" == "GO" ] ; then
        logger "Stopping P2P GO mode."
        /sbin/ifconfig "$1" down
        sleep 2
        kill_process dnsmasq /tmp/dnsmasq-$1.pid
    elif [ "$2" == "client" ]; then
        logger "Stopping P2P client mode."
        kill_process udhcpc /tmp/dnsmasq-$1.pid
    else
        logger "Invalid role, must be GO or client"
        exit 1
    fi
else
    logger "Must ADD or REMOVE an interface."
    exit 1
fi

exit 0

