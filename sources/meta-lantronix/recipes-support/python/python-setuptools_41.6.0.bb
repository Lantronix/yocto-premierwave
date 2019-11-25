SUMMARY = "Downloads, builds, installs, upgrades, and uninstalls Python packages"
HOMEPAGE = "https://pypi.python.org/pypi/setuptools"
SECTION = "devel/python"
#LICENSE = "MIT License"
#LIC_FILES_CHKSUM = "file://setup.py;beginline=78;endline=78;md5=68b329da9893e34099c7d8ad5cb9c940"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;beginline=1;endline=19;md5=9a33897f1bca1160d7aad3835152e158"

SRCNAME = "setuptools"

PROVIDES = "python-distribute"

DEPENDS += "python"
DEPENDS_class-native += "python-native"

inherit distutils

SRC_URI = "https://pypi.python.org/packages/source/s/setuptools/setuptools-${PV}.zip"
SRC_URI[md5sum] = "5585a55bfc28474ef13cc0b1819c5a46"
SRC_URI[sha256sum] = "6afa61b391dcd16cb8890ec9f66cc4015a8a31a6e1c2b4e0c464514be1a3d722"

S = "${WORKDIR}/${SRCNAME}-${PV}"


DISTUTILS_INSTALL_ARGS += "--install-lib=${D}${libdir}/${PYTHON_DIR}/site-packages"

do_install_prepend() {
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages
}

RDEPENDS_${PN} = "\
  python-stringold \
  python-email \
  python-shell \
  python-distutils \
  python-compression \
"

RDEPENDS_${PN}_class-native = "\
  python-distutils \
  python-compression \
"

RREPLACES_${PN} = "python-distribute"
RPROVIDES_${PN} = "python-distribute"
RCONFLICTS_${PN} = "python-distribute"

BBCLASSEXTEND = "native nativesdk"
