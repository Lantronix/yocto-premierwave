DESCRIPTION = "MiniUPnPD for Linux"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=ea6862bcd71f55f20a60e6580b26064d"

SRC_URI = "http://miniupnp.tuxfamily.org/files/${BP}.tar.gz \
           file://${BP}-001-ltrx-allow_linklocal.patch \
           file://${BP}-002-ltrx-config.patch \
           file://${BP}-003-ltrx-genxml.patch \
           file://${BP}-004-ltrx-makefile.patch \
           file://${BP}-005-ltrx-supportv6_add_only.patch \
           file://${BP}-006-ltrx-suppress_log.patch \
           file://${BP}-007-iptables.patch \
           file://${BP}-008-logmask.patch \
           file://${BP}-009-ignore-bridged-addr.patch \
"

SRC_URI[md5sum] = "e9e53869bb725e931cae74b20d4a39be"
SRC_URI[sha256sum] = "e2bc8040e912574af122cd295012b566b574032e39d76086c5ad1968bbc9aba3"

inherit autotools-brokensep

DEPENDS = "iptables libnfnetlink"

do_compile() {
    CONFIG_OPTIONS="--ipv6" make -f Makefile.linux
}

do_install() {
    install -D -m 0755 ${B}/${PN} ${D}${sbindir}/${PN}
}
