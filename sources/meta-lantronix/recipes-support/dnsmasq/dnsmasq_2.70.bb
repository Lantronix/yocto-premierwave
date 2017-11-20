require dnsmasq.inc

SRC_URI += "\
    file://lua.patch \
    file://0001-lease_ip_display.patch \
"

SRC_URI[dnsmasq-2.70.md5sum] = "d6afbf46cd80bb0b7f6ed1404733ed4f"
SRC_URI[dnsmasq-2.70.sha256sum] = "8eb7bf53688d6aaede5c90cfd2afcce04803a4efbddfbeecc6297180749e98af"
