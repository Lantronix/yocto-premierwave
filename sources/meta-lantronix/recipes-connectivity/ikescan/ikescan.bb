# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)
#
# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=94d55d512a9ba36caa9b7df079bae19f"

SRC_URI = "file://ike-scan-1.9.tar.gz \
           file://ike-scan-1.9-01-build.patch \
           file://ike-scan-1.9-02-alignment.patch \
"

FILES_${PN} += "/usr/share/ike-scan/"

# Modify these as desired
#PV = "1.0+git${SRCPV}"
#SRCREV = "fcb1672d2026669b7fd05a252eaf046e993eb291"

S = "${WORKDIR}/ike-scan-1.9"

# NOTE: if this software is not capable of being built in a separate build directory
# from the source, you should replace autotools with autotools-brokensep in the
# inherit line
#inherit autotools

do_configure() {
    ./configure --build=x86_64-linux --host=arm-poky-linux-gnueabi --target=arm-poky-linux-gnueabi --prefix=/usr --exec_prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib/ikescan --datadir=/usr/share --sysconfdir=/etc --sharedstatedir=/com --localstatedir=/var --libdir=/usr/lib --includedir=/usr/include --oldincludedir=/usr/include --infodir=/usr/share/info --mandir=/usr/share/man --disable-silent-rules --disable-dependency-tracking --with-libtool-sysroot=${STAGING_DIR_TARGET}
}

do_install() {
    install -m 0755 -d ${D}${bindir} ${D}/usr/share/ike-scan/
    install -m 0755 ${S}/ike-scan ${D}${bindir}
    install -m 0755 ${S}/ike-vendor-ids ${D}/usr/share/ike-scan/
    install -m 0755 ${S}/ike-backoff-patterns ${D}/usr/share/ike-scan/
    rm -rf ${D}/usr/share/ike-scan/checks
    rm -f  ${D}/usr/share/ike-scan/psk-crack-dictionary
}
# Specify any options you want to pass to the configure script using EXTRA_OECONF:
EXTRA_OECONF = ""

