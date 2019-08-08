# modify "../../recipes-bsp/images/ltrx-image.bbappend" to:
# (1) build this application into the ROM image,
# (2) start this application during bootup via inittab if needed

DESCRIPTION = "Time synchronization script"
LICENSE = "CLOSED"
RDEPENDS_${PN} = "bash"

SRC_URI = "file://timesync.sh file://timesync.service"

S = "${WORKDIR}"
inherit systemd

do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
    install -Dm 0644 ${WORKDIR}/timesync.service ${D}/${systemd_unitdir}/system/timesync.service
    install -Dm 0755 ${WORKDIR}/timesync.sh ${D}/${bindir}/timesync.sh
}

SYSTEMD_SERVICE_${PN} = "timesync.service"

