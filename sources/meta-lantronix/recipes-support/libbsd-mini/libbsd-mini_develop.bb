DESCRIPTION = "Minimal implementation of libbsd"
LICENSE = "CLOSED"

SRC_URI = "file://libbsd-mini-develop.zip"
SRC_URI[md5sum] = "xxx"
SRC_URI[sha256sum] = "xxx"

# runs on the host
BBCLASSEXTEND =+ "native nativesdk"

inherit autotools-brokensep

do_install() {
    install -d -m 0644 ${D}${includedir}/bsd
    install -m 0644 ${S}/include/bsd/* ${D}${includedir}/bsd
    install -d -m 0755 ${D}${libdir}
    install -m 0755 ${B}/libbsd-mini.so* ${D}${libdir}
}

ALLOW_EMPTY_${PN} = "1"
FILES_${PN}-dev = "${includedir}/bsd ${libdir}/libbsd-mini.so*"
