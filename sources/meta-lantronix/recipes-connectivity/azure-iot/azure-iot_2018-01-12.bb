DESCRIPTION = "Azure IoT SDK"
LICENSE = "CLOSED"

SRC_URI = "gitsm://github.com/Azure/azure-iot-sdk-c.git;--branch=${SRCREV} \
          file://iothub_client.so \
          file://libboost_python.so.1.58.0 \
"

SRC_URI[md5sum] = "xxx"
SRC_URI[sha256sum] = "xxx"

DEPENDS += "openssl curl util-linux"

SRCREV = "2018-01-12"

S = "${WORKDIR}/azure"

do_install() {
    install -d -m 0755 ${D}/ltrx_user/
    install -d -m 0755 ${D}/usr/lib/
    install -m 0755 ${WORKDIR}/iothub_client.so ${D}/ltrx_user/
    install -m 0755 ${WORKDIR}/libboost_python.so.1.58.0 ${D}/usr/lib/
}

FILES_${PN} = "${WORKDIR}/iothub_client.so /ltrx_user/iothub_client.so"
FILES_${PN} += "ltrx_user/iothub_client.so"
FILES_${PN} += "/usr/lib/libboost_python.so.1.58.0"
FILES_${PN}-dbg += "/ltrx_user/.debug"



