SUMMARY = "A simple Modbus/TCP client library for Python"
SECTION = "devel/python"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=c7e415a167afb41ded0cb9a97e365917"

DEPENDS = "python"

PR = "r0"

SRC_URI = "file://pyModbusTCP-${PV}.tar.gz"

SRC_URI[md5sum] = "1b2ec44e9537e14dcb8a238ea3eda451"
SRC_URI[sha256sum] = "d9acf6457bc26d3c784caa5d7589303afe95e980ceff860ec2a4051038bc261e"

S = "${WORKDIR}/pyModbusTCP-${PV}"

do_install_append() {
    cp -r ${WORKDIR}/pyModbusTCP-${PV}/pyModbusTCP ${D}/usr/lib/python2.7/site-packages/
}

inherit distutils
