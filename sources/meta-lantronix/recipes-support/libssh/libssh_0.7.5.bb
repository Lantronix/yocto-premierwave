SUMMARY = "Multiplatform C library implementing the SSHv2 and SSHv1 protocol"
HOMEPAGE = "http://www.libssh.org"
SECTION = "libs"

DEPENDS = "zlib openssl"

LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://COPYING;md5=388a4fb1dea8ceae0be78ba9b01fc139"

SRC_URI = "file://libssh-${PV}.tar.xz \
           file://libssh-01-build-in-source.patch  \
           file://libssh-02-fips-crypto.patch \
          "
SRCREV = "053f72c671a83b32238dc01b56a66752fb15b7ec"
S = "${WORKDIR}/libssh-${PV}/"

EXTRA_OECMAKE = " \
    -DWITH_GCRYPT=OFF \
    -DWITH_PCAP=OFF \
    -DWITH_SFTP=ON \
    -DWITH_ZLIB=ON \
    -DWITH_STACK_PROTECTOR=OFF \
    -DWITH_EXAMPLES=OFF \
    -DLIB_SUFFIX=${@d.getVar('baselib', True).replace('lib', '')} \
    "

PACKAGECONFIG ??=""
PACKAGECONFIG[gssapi] = "-DWITH_GSSAPI=1, -DWITH_GSSAPI=0, krb5, "

PACKAGECONFIG_remove = "gssapi"

inherit cmake

do_configure_prepend () {
    # Disable building of examples
    sed -i -e '/add_subdirectory(examples)/s/^/#DONOTWANT/' ${S}/CMakeLists.txt \
        || bbfatal "Failed to disable examples"
}

FILES_${PN}-dev += "${libdir}/cmake"
