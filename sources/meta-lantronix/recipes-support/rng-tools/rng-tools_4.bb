SUMMARY = "Random number generator daemon"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=94d55d512a9ba36caa9b7df079bae19f"
DEPENDS_append_libc-uclibc = " argp-standalone"

SRC_URI = "http://archive.ubuntu.com/ubuntu/pool/universe/r/rng-tools/rng-tools_4.orig.tar.gz \
           file://init \
           file://default"

SRC_URI[md5sum] = "e5c3b814619e97734842892a93dafd59"
SRC_URI[sha256sum] = "114f225a419cf367bc91732ef6e9cbac64f4df01f9cf10c88d792baf31feea68"

# As the recipe doesn't inherit systemd.bbclass, we need to set this variable
# manually to avoid unnecessary postinst/preinst generated.
python () {
    if not bb.utils.contains('DISTRO_FEATURES', 'sysvinit', True, False, d):
        d.setVar("INHIBIT_UPDATERCD_BBCLASS", "1")
}

inherit autotools update-rc.d

do_install_append() {
    # Only install the init script when 'sysvinit' is in DISTRO_FEATURES.
    if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
        install -d "${D}${sysconfdir}/init.d"
        install -m 0755 ${WORKDIR}/init ${D}${sysconfdir}/init.d/rng-tools
        sed -i -e 's,/etc/,${sysconfdir}/,' -e 's,/usr/sbin/,${sbindir}/,' \
            ${D}${sysconfdir}/init.d/rng-tools

        install -d "${D}${sysconfdir}/default"
        install -m 0644 ${WORKDIR}/default ${D}${sysconfdir}/default/rng-tools
    fi
}

INITSCRIPT_NAME = "rng-tools"
INITSCRIPT_PARAMS = "start 30 S . stop 30 0 6 1 ."
