DESCRIPTION = "Wide DHCPv6 is a program for supporting the DHCPv6 protocol."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYRIGHT;md5=c180fcd362bda6abeae4ca1f7d7ac3fc"
DEPENDS += "flex"

SRC_URI = "${SOURCEFORGE_MIRROR}/project/${BPN}/${BPN}/${BPN}-${PV}/${BPN}-${PV}.tar.gz \
           file://0001-dhcp6c.patch \
           file://0002-dhcp6s.patch \
           file://0003-dhcp6.patch \
           file://0004-printf.patch \
           file://0005-dhcp-type3.patch \
           file://0006-config-close.patch \
          "
SRC_URI[md5sum] = "1011e165850fe40d3e332dad2bfd30b3"
SRC_URI[sha256sum] = "55a66174a1edeabd90029b83cb3fff8e0b63718a556ce95b97d464a87fd1bd81"

inherit autotools-brokensep

# -DLTRX_FIXES
EXTRA_OECONF += " \
                 ac_cv_func_setpgrp_void=yes \
                 ac_cv_func_strlcpy=no \
                 ac_cv_func_daemon=yes \
                 ac_cv_func_warnx=yes \
                 ac_cv_func_arc4random=no \
                "

do_install() {
    install -D -m 0755 ${B}/dhcp6c ${D}${bindir}/dhcp6c
    install -D -m 0755 ${B}/dhcp6s ${D}${bindir}/dhcp6s
}

FILES_${PN} += "${bindir}/dhcp6c ${bindir}/dhcp6s"
