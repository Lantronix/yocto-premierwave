DESCRIPTION = ".rom image creation tool"
LICENSE = "CLOSED"

SRC_URI = "file://ltrx-mkimage file://ltrx_image.h"
S = "${WORKDIR}"
PV = "${LTRX_PRODUCT_VERSION}"

# runs on the host
BBCLASSEXTEND = "native"

do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
	install -D -m 0755 ${B}/ltrx-mkimage ${D}${bindir}/ltrx-mkimage
	install -D -m 0755 ${S}/ltrx_image.h ${D}${includedir}/ltrx_image.h
}

