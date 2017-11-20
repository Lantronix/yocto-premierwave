# modify "../../recipes-bsp/images/ltrx-image.bbappend" to:
# (1) build this application into the ROM image,
# (2) start this application during bootup via inittab if needed

DESCRIPTION = "Lantronix apps SDK Hello World example recipe"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=f57fbe2c512bc7df7533be890770a474"

SRC_URI = "file://helloworld.c file://Makefile file://COPYING.MIT"

S = "${WORKDIR}"

app = "helloworld"

do_compile() {
    make ${app}
}

do_install() {
    install -D -m 0755 ${B}/${app} ${D}/bin/${app}
}

