#!/bin/sh
# dhcp6c-script for Debian/Ubuntu. Jérémie Corbier, April, 2006.
# resolvconf support by Mattias Guns, May 2006.
interface="wlan0"
logger -p "user.debug" "dhcp6-script interface ${interface}, new_ntp_servers: ${new_ntp_servers}, new_domain_name_servers: ${new_domain_name_servers}"

LTRX_NET_COMMON="/etc/sysconfig/network-scripts/ltrx-network-common.script"
if [ ! -f $LTRX_NET_COMMON ]; then
    logger -p "user.err" "dhcp6-script wlan0: missing network script"
    exit 1
fi

DNS_CONF="/etc/sysconfig/dhcp6_dns_${interface}"
rm -f $DNS_CONF
for nameserver in $new_domain_name_servers ; do
    echo $nameserver >> $DNS_CONF
done
touch $DNS_CONF

DOMAIN_CONF="/etc/sysconfig/dhcp6_domain_${interface}"
rm -f $DOMAIN_CONF
[ -n "$new_domain_name" ] && echo "DHCP_DOMAIN=\"$new_domain_name\"" > $DOMAIN_CONF
touch $DOMAIN_CONF

if [ -e /tmp/bridging.active ] ; then
    ebtables -t broute -D BROUTING --concurrent -i wlan0 --proto ipv6 --ip6-protocol udp --ip6-source-port 546:547 --ip6-destination-port 546:547 -j DROP
    while true; do
        nrules=`ebtables -t broute --concurrent -L  | grep  'ip6-sport 546:547' | grep 'ip6-dport 546:547' | wc -l`
        if [ $nrules -eq 0 ]; then
            break
        fi
        ebtables -t broute -D BROUTING --concurrent -i wlan0 --proto ipv6 --ip6-protocol udp --ip6-source-port 546:547 --ip6-destination-port 546:547 -j DROP
    done
fi

#run network script to update resolv.conf and routes etc.
. $LTRX_NET_COMMON
ltrx_start_network ${interface}
exit 0
