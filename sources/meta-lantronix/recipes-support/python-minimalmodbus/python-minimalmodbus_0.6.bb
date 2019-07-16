SUMMARY = "Easy-to-use Modbus RTU and Modbus ASCII implementation for Python"
SECTION = "devel/python"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENCE.txt;md5=27da4ba4e954f7f4ba8d1e08a2c756c4"

DEPENDS = "python"
RDEPENDS_${PN} = "python-pyserial"

PR = "r0"

SRC_URI = "${SOURCEFORGE_MIRROR}/project/minimalmodbus/${PV}/MinimalModbus-${PV}.tar.gz"

SRC_URI[md5sum] = "4bc296fab36a2b2b8c8cf77c0b6f6d7a"
SRC_URI[sha256sum] = "148139f068eb6d376e6f19e93ff10ba91f93735d9848c91082d85c176851b5b1"

S = "${WORKDIR}/MinimalModbus-${PV}"

inherit distutils
