SUMMARY = "Multipurpose relay for bidirectional data transfer"
DESCRIPTION = "Socat is a relay for bidirectional data \
transfer between two independent data channels."
HOMEPAGE = "http://www.dest-unreach.org/socat/"

SECTION = "console/network"

DEPENDS = "openssl readline"

LICENSE = "GPL-2.0-with-OpenSSL-exception"
LIC_FILES_CHKSUM = "file://COPYING;md5=18810669f13b87348459e611d31ab760 \
                    file://README;beginline=257;endline=287;md5=ec25a7504db3d05958c249592d80eadc"

SRC_URI = "file://socat-2.0.0-b7.tar.bz2 \
"

SRC_URI[md5sum] = "ff56576703dfdeac221357a348c30760"
SRC_URI[sha256sum] = "c804579db998fb697431c82829ae03e6a50f342bd41b8810332a5d0661d893ea"

inherit autotools

EXTRA_AUTORECONF += "--exclude=autoheader"

EXTRA_OECONF += "ac_cv_have_z_modifier=yes \
                 ac_cv_header_bsd_libutil_h=no \
                 sc_cv_termios_ispeed=no \
                 ${TERMBITS_SHIFTS} \
"

TERMBITS_SHIFTS ?= "sc_cv_sys_crdly_shift=9 \
                    sc_cv_sys_tabdly_shift=11 \
                    sc_cv_sys_csize_shift=4"

TERMBITS_SHIFTS_powerpc = "sc_cv_sys_crdly_shift=12 \
                           sc_cv_sys_tabdly_shift=10 \
                           sc_cv_sys_csize_shift=8"

TERMBITS_SHIFTS_powerpc64 = "sc_cv_sys_crdly_shift=12 \
                             sc_cv_sys_tabdly_shift=10 \
                             sc_cv_sys_csize_shift=8"

PACKAGECONFIG_class-target ??= "tcp-wrappers"
PACKAGECONFIG ??= ""
PACKAGECONFIG[tcp-wrappers] = "--enable-libwrap,--disable-libwrap,tcp-wrappers"

PACKAGECONFIG_remove = "tcp-wrappers"

do_install_prepend () {
    mkdir -p ${D}${bindir}
    install -d ${D}${bindir} ${D}${mandir}/man1
}

BBCLASSEXTEND = "native nativesdk"

