DESCRIPTION = "JSON-C - A JSON implementation in C"
HOMEPAGE = "http://oss.metaparadigm.com/json-c/"

LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://License.txt;md5=cebe661e002daaeae68254a3c8b7a7df"

SRC_URI = "file://libjson_7.6.1.zip \
	file://libjson-0001-fix-broken-makefile.patch \
"
SRC_URI[md5sum] = "82f3fcbf9f8cf3c4e25e1bdd77d65164"
SRC_URI[sha256sum] = "07267a3951038ee2e02d26cc41bf8e275668c38f751240d3e78dc979182e7376"

S = "${WORKDIR}/libjson"


inherit autotools
BBCLASSEXTEND =+ "native nativesdk"
do_compile() {
	cp -r ${WORKDIR}/libjson/* ${B}/
	mkdir -p ${B}/Objects_shared \
	${B}/_internal/Source/Dependencies
        make SHARED=1 CC="${CC}" 
}
do_install() {
    install -d -m 0644 ${D}${includedir}/libjson
    install -m 0644 ${WORKDIR}/libjson/libjson.h ${D}${includedir}/libjson
    install -d -m 0644 ${D}${includedir}/libjson/_internal/Source
    install -m 0644 ${WORKDIR}/libjson/_internal/Source/*.h ${D}${includedir}/libjson/_internal/Source
    install -m 0644 ${WORKDIR}/libjson/JSONOptions.h ${D}${includedir}/libjson
    install -d -m 0644 ${D}${includedir}/libjson/_internal/Source/JSONDefs
    install -m 0644 ${WORKDIR}/libjson/_internal/Source/JSONDefs/*.h ${D}${includedir}/libjson/_internal/Source/JSONDefs
    install -d -m 0755 ${D}${libdir}
    install -m 0755 ${B}/libjson.so* ${D}${libdir}
}

FILES_${PN}-dev = "${includedir}"

FILES_${PN} = "${B}/libjson.so* ${libdir}/libjson_7.6.21"
FILES_${PN} += "${libdir}/*" 

