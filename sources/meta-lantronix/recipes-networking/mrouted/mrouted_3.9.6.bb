DECRIPTION = "DVMRP multicast routing daemon"
HOMEPAGE = "http://troglobit.com/mrouted.shtml"
SECTION = "network"
LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=5a159dd66aeb92b678404daa0fedfa04"
DEPENDS = "bison-native"

SRC_URI = "file://${BP}.tar.bz2 \
           file://${BP}-bcopy.patch \
           file://${BP}-config-mk.patch \
           file://001-mroute-fix-bridge.patch \
           file://002-DVMRP-report-missing-subnet.patch \
"
SRC_URI[md5sum] = "f9f860e15c24124c4ca4827d938658f2"
SRC_URI[sha256sum] = "8f6bdbd6eec1da3f6b0ca651a0484d7bdf7d7ead6c5d47abbaef0dfed949eccf"

EXTRA_OEMAKE = " \
        datadir=${datadir} \
        mandir=${mandir} \
        prefix=${prefix} \
        sysconfdir=${sysconfdir} \
        DESTDIR=${D} \
"

do_install() {
    install -D -m 0755 ${B}/${PN} ${D}${sbindir}/${PN}
}
