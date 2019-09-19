#!/bin/sh
# dhcp6c-script for Debian/Ubuntu. Jérémie Corbier, April, 2006.
# resolvconf support by Mattias Guns, May 2006.
interface="eth0"
logger -p "user.debug" "dhcp6-script interface ${interface}, new_ntp_servers: ${new_ntp_servers}, new_domain_name_servers: ${new_domain_name_servers}"

LTRX_NET_COMMON="/etc/sysconfig/network-scripts/ltrx-network-common.script"
if [ ! -f $LTRX_NET_COMMON ]; then
    logger -p "user.err" "dhcp6-script eth0: missing network script"
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

#run network script to update resolv.conf and routes etc.
. $LTRX_NET_COMMON
ltrx_start_network ${interface}
restart_l3_bridging ${interface}
exit 0
