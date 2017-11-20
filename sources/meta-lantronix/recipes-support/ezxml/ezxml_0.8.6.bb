DESCRIPTION = "C library for parsing XML documents inspired by simpleXML for PHP"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://license.txt;md5=d4cda95c0365c4d0c092701007508eab"

SRC_URI = "${SOURCEFORGE_MIRROR}/ezxml/ezXML/ezXML%20${PV}/ezxml-${PV}.tar.gz \
           file://ezxml-${PV}-allow-compiler-override.patch \
"

SRC_URI[md5sum] = "e22ae17a0bd82dfa2a66f9876f1a8fd7"
SRC_URI[sha256sum] = "a68d52257dcb0ff2ad3d71c8c64311edb8030254bb8b581e229aeaba6231cdf9"

S = "${WORKDIR}/${PN}"
#B = "${S}"

inherit autotools-brokensep

do_install() {
    install -D -m 0644 ${B}/libezxml.a ${D}${libdir}/libezxml.a
    install -D -m 0644 ${S}/ezxml.h ${D}${includedir}/ezxml.h
}

# no main package, only -staticdev
ALLOW_EMPTY_${PN} = "1"
