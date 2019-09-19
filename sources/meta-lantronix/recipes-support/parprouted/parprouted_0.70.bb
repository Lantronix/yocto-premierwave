# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)
#
# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=0636e73ff0215e8d672dc4c32c317bb3"

SRC_URI = "file://parprouted_${PV}.tar.gz \
"

# Modify these as desired
#PV = "1.0+git${SRCPV}"
#SRCREV = "fcb1672d2026669b7fd05a252eaf046e993eb291"

S = "${WORKDIR}/parprouted-0.7"

# NOTE: if this software is not capable of being built in a separate build directory
# from the source, you should replace autotools with autotools-brokensep in the
# inherit line
#inherit autotools

app = "parprouted"

do_compile() {
    make ${app}
}

do_install() {
    install -D -m 0755 ${B}/${app} ${D}/bin/${app}
}

# Specify any options you want to pass to the configure script using EXTRA_OECONF:
EXTRA_OECONF = ""

