SUMMARY = "Pyzmq provides Zero message queue access for the Python language"
HOMEPAGE = "http://zeromq.org/bindings:python"
LICENSE = "BSD & LGPL-3.0"
LIC_FILES_CHKSUM = "file://COPYING.BSD;md5=11c65680f637c3df7f58bbc8d133e96e \
                    file://COPYING.LESSER;md5=12c592fa0bcfff3fb0977b066e9cb69e"

DEPENDS = "zeromq"

FILESEXTRAPATHS_prepend := "${THISDIR}/python-pyzmq:"

SRC_URI += "file://club-rpath-out.patch"

RDEPENDS_${PN} += "${PYTHON_PN}-multiprocessing"

FILES_${PN}-dbg =+ "${PYTHON_SITEPACKAGES_DIR}/zmq/backend/cython/.debug"

do_compile_prepend() {
    echo [global] > ${S}/setup.cfg
    echo zmq_prefix = ${STAGING_DIR_HOST} >> ${S}/setup.cfg
    echo have_sys_un_h = True >> ${S}/setup.cfg
    echo skip_check_zmq = True >> ${S}/setup.cfg
    echo libzmq_extension = False >> ${S}/setup.cfg
    echo no_libzmq_extension = True >> ${S}/setup.cfg
}

SRC_URI[md5sum] = "d850bbab7c7f558b91c95b19028ff4b8"
SRC_URI[sha256sum] = "8c69a6cbfa94da29a34f6b16193e7c15f5d3220cb772d6d17425ff3faa063a6d"

PYPI_PACKAGE = "pyzmq"

inherit pypi setuptools3
