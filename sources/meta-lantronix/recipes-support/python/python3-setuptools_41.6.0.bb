SUMMARY = "Download, build, install, upgrade, and uninstall Python packages"
HOMEPAGE = "http://packages.python.org/setuptools"
SECTION = "devel/python"
LICENSE = "PSF"
LIC_FILES_CHKSUM = "file://setup.py;beginline=78;endline=78;md5=068d2294d9802ce67087155718d73797"

SRCNAME = "setuptools"
DEPENDS += "python3"
DEPENDS_class-native += "python3-native"

SRC_URI = "https://pypi.python.org/packages/source/s/setuptools/setuptools-${PV}.zip"
SRC_URI[md5sum] = "5585a55bfc28474ef13cc0b1819c5a46"
SRC_URI[sha256sum] = "6afa61b391dcd16cb8890ec9f66cc4015a8a31a6e1c2b4e0c464514be1a3d722"

S = "${WORKDIR}/${SRCNAME}-${PV}"

inherit distutils3

DISTUTILS_INSTALL_ARGS += "--install-lib=${D}${libdir}/${PYTHON_DIR}/site-packages"

do_install_prepend() {
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages
}
#
#  The installer puts the wrong path in the setuptools.pth file.  Correct it.
#
do_install_append() {
    rm ${D}${PYTHON_SITEPACKAGES_DIR}/setuptools.pth
    mv ${D}${bindir}/easy_install ${D}${bindir}/easy3_install
    echo "./${SRCNAME}-${PV}-py${PYTHON_BASEVERSION}.egg" > ${D}${PYTHON_SITEPACKAGES_DIR}/setuptools.pth
}

RDEPENDS_${PN} = "\
  python3-distutils \
  python3-compression \
"
RDEPENDS_${PN}_class-target = "\
  python3-ctypes \
  python3-distutils \
  python3-email \
  python3-importlib \
  python3-numbers \
  python3-compression \
  python3-shell \
  python3-subprocess \
  python3-textutils \
  python3-pkgutil \
  python3-threading \
  python3-misc \
  python3-unittest \
  python3-xml \
"
BBCLASSEXTEND = "native"
