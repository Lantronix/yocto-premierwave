DESCRIPTION = "Support MQ client"
LICENSE = "EPL-1.0 | EDL-1.0"
LIC_FILES_CHKSUM = " \
        file://edl-v10;md5=3adfcc70f5aeb7a44f3f9b495aa1fbf3 \
        file://epl-v10;md5=659c8e92a40b6df1d9e3dccf5ae45a08 \
        file://notice.html;md5=a00d6f9ab542be7babc2d8b80d5d2a4c \
        file://about.html;md5=dcde438d73cf42393da9d40fabc0c9bc \
"
SRC_URI = "file://paho.mqtt.embedded-c.tar.gz"
SRC_URI[md5sum] = "xxx"
SRC_URI[sha256sum] = "xxx"

S = "${WORKDIR}/paho.mqtt.embedded-c"

inherit autotools-brokensep

do_compile() {
        make CC="${CC}" 
}

do_install() {
    install -d -m 0755 ${D}${libdir}
    install -m 0755 ${B}/build/output/libpaho-embed-mqtt3c.so* ${D}${libdir}
    install -d -m 0644 ${D}${includedir}/MQTTPacket
    install -m 0644 ${WORKDIR}/paho.mqtt.embedded-c/MQTTPacket/src/*.h ${D}${includedir}/MQTTPacket
    install -m 0644 ${WORKDIR}/paho.mqtt.embedded-c/MQTTPacket/src/*.h ${D}${includedir}
}
