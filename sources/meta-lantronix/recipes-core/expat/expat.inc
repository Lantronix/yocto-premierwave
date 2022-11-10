SUMMARY = "A stream-oriented XML parser library"
DESCRIPTION = "Expat is an XML parser library written in C. It is a stream-oriented parser in which an application registers handlers for things the parser might find in the XML document (like start tags)"
HOMEPAGE = "http://github.com/libexpat/libexpat/releases/download/"
SECTION = "libs"
LICENSE = "MIT"

SRC_URI = "${HOMEPAGE}/R_2_4_8/expat-${PV}.tar.gz \
          file://buildroot-libtool-v2.4.4.patch"

inherit autotools lib_package gzipnative

FILES_${PN} += "${libdir}/*"

# This package uses an archive format known to have issue with some
# versions of gzip
do_unpack[depends] += "gzip-native:do_populate_sysroot"

do_configure_prepend () {
	rm -f ${S}/conftools/libtool.m4
}

BBCLASSEXTEND = "native nativesdk"
