SUMMARY = "Serial Port Support for Python"
SECTION = "devel/python"

LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=d476d94926db6e0008a5b3860d1f5c0d"

SRCNAME = "pyserial"

SRC_URI = "https://pypi.python.org/packages/source/p/pyserial/pyserial-${PV}.tar.gz"
SRC_URI[md5sum] = "ed6183b15519a0ae96675e9c3330c69b"
SRC_URI[sha256sum] = "6e2d401fdee0eab996cf734e67773a0143b932772ca8b42451440cfed942c627"
S = "${WORKDIR}/${SRCNAME}-${PV}"

inherit setuptools

