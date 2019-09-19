SUMMARY = "Open-source json-c library"
SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://cjson-58.tar.gz \
"
SRC_URI += "\
  file://001-cjson-lax.patch \
"
S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} -shared -fPIC -lm cJSON.c -o libcJSON.so
}

do_install() {
	install -D cJSON.h ${D}${includedir}/cJSON.h
    install -D libcJSON.so ${D}${libdir}/libcJSON.so
}

# no main package, only -dev
ALLOW_EMPTY_${PN} = "1"

BBCLASSEXTEND =+ "native nativesdk"
