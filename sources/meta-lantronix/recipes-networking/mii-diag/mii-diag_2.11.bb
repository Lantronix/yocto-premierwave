DESCRIPTION = "Examines and sets the MII registers of network cards."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://mii-diag.c;beginline=16;endline=20;md5=ddb358f6c9150934061be4aed16c17d9"
SECTION = "console/network"

S = "${WORKDIR}/${BPN}-${PV}.orig"

#PACKAGE_STRIP = "no"

SRC_URI = "${DEBIAN_MIRROR}/main/m/${BPN}/${BPN}_${PV}.orig.tar.gz;name=tarball \
           ${DEBIAN_MIRROR}/main/m/${BPN}/${BPN}_${PV}-3.diff.gz;name=patch \
          "
SRC_URI[tarball.md5sum] = "2c0cc0cd29c80f86921e6f300709bf81"
SRC_URI[tarball.sha256sum] = "c690e87e6010607593c1cc2ccd5c481eb3be179387220ad445d8ab83d73ad41c"
SRC_URI[patch.md5sum] = "d9da543e47bb1db1c54c2c3d9240f747"
SRC_URI[patch.sha256sum] = "b49fd3e7c0c446a949c3ca246cc441824d7ef5a61530f884756a82b0b2997190"

FILES_${PN} = "${sbindir}/mii-diag"

do_unpack[cleandirs] += "${S}"

do_debian_patches() {
    if [ -d ${S}/debian/patches ]; then
        support/scripts/apply-patches.sh ${S} ${S}/debian/patches *.patch
    fi
}
addtask debian_patches after do_patch before do_compile

do_install() {
	install -D -m 0755 ${S}/mii-diag ${D}${sbindir}/mii-diag
}
