SECTION = "console/network"
DESCRIPTION = "Openswan is an Open Source implementation of IPsec for the \
Linux operating system."
HOMEPAGE = "http://www.openswan.org"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=65b1c7b9e2985a7204bd3aba4ddc7132"
DEPENDS = "gmp flex-native bison-native perl"
RRECOMMENDS_${PN} = "kernel-module-ipsec"
RDEPENDS_append_nylon = "perl"
#PR = "r3"

SRC_URI = "file://openswan-${PV}.tar.gz"

EXTRA_OEMAKE = "DESTDIR=${D} \
                USERCOMPILE='${CFLAGS}' \
                USERLINK='${LDFLAGS}' \
                FINALCONFDIR=${sysconfdir}/ipsec \
                FINALLIBDIR=${libdir}/ipsec \
                FINALLIBEXECDIR=${libexecdir}/ipsec \
                FINALSBINDIR=${sbindir} \
                INC_RCDEFAULT=${sysconfdir}/init.d \
                INC_USRLOCAL=${prefix} \
                INC_MANDIR=share/man WERROR=''"

do_compile () {
	oe_runmake programs
}

do_install () {
	oe_runmake install
}

PACKAGES =+ "${PN}-examples ${PN}-test ${PN}-klips"

INSANE_SKIP_${PN} = "installed-vs-shipped "
PARALLEL_MAKE = ""

FILES_${PN} = "${sysconfdir} ${libdir}/ipsec/* ${sbindir}/* ${libexecdir}/ipsec/*"
FILES_${PN}-dbg += "${libdir}/ipsec/.debug ${libexecdir}/ipsec/.debug"

CONFFILES_${PN} = "${sysconfdir}/ipsec/ipsec.conf"

FILES_${PN}-examples = "${sysconfdir}/ipsec.d/examples"

# KLIPS requires some binaries and scripts that NETKEY users don't need.
FILES_${PN}-klips = " \
        ${libexecdir}/ipsec/eroute \
        ${libexecdir}/ipsec/klipsdebug \
        ${libexecdir}/ipsec/spi \
        ${libexecdir}/ipsec/spigrp \
        ${libexecdir}/ipsec/tncfg \
        ${libdir}/ipsec/_updown.klips \
"

FILES_${PN}-test = " \
        ${libexecdir}/ipsec/showpolicy \
        ${libexecdir}/ipsec/verify \
"

SRC_URI[md5sum] = "da2e8b02ecc30a408cc5766767fef84f"
SRC_URI[sha256sum] = "f9ebb395cb0f717dc43942662ab65161035a99dbf8e680c1d1511d4de130d0fb"

RDEPENDS_${PN} += "perl"
