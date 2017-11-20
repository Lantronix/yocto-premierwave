SUMMARY = "Downloads, builds, installs, upgrades, and uninstalls Python packages"
HOMEPAGE = "https://pypi.python.org/pypi/setuptools"
SECTION = "devel/python"
LICENSE = "Python-2.0 | ZPL-2.0"
LIC_FILES_CHKSUM = "file://setup.py;beginline=78;endline=78;md5=b8f378609eb7bdaf7092ac8307cdc29f"

SRCNAME = "setuptools"

PROVIDES = "python-distribute"

DEPENDS += "python"
DEPENDS_class-native += "python-native"

inherit distutils

SRC_URI = "https://pypi.python.org/packages/source/s/setuptools/setuptools-${PV}.tar.gz \
	file://python-setuptools-add-executable.patch \
"
SRC_URI[md5sum] = "3fb7b3abb02d1d1eb9dc45e1c53e1539"
SRC_URI[sha256sum] = "3b5b2969a8c3edab601861e1281fb8ffa37920bf71d8c40e02506e33403748a9"

S = "${WORKDIR}/${SRCNAME}-${PV}"

DISTUTILS_INSTALL_ARGS += "--install-lib=${D}${libdir}/${PYTHON_DIR}/site-packages"

do_install_append() {
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools-2.1.2-py2.7.egg-info
    install -m 0644 ${WORKDIR}/setuptools-${PV}/setuptools.egg-info/* ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools-2.1.2-py2.7.egg-info/
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/command
    install -m 0644 ${WORKDIR}/setuptools-${PV}/setuptools/command/*.pyc ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/command/
    install -d ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/tests
    install -m 0644 ${WORKDIR}/setuptools-${PV}/setuptools/tests/*.py* ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/tests/
    install -m 0644 ${WORKDIR}/setuptools-${PV}/*.pyc ${D}/${libdir}/${PYTHON_DIR}/site-packages/
    install -m 0644 ${WORKDIR}/setuptools-${PV}/setuptools/*.pyc ${D}/${libdir}/${PYTHON_DIR}/site-packages/setuptools/
}

BBCLASSEXTEND = "native nativesdk"
