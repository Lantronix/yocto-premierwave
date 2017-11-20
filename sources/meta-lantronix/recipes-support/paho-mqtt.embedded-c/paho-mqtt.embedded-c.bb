DESCRIPTION = "Support MQ client"
LICENSE = "CLOSED"

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
