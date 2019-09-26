require busybox.inc

PR = "r32"

SRC_URI = "http://www.busybox.net/downloads/busybox-${PV}.tar.bz2;name=tarball \
           file://get_header_tar.patch \
           file://busybox-appletlib-dependency.patch \
           file://busybox-udhcpc-no_deconfig.patch \
           file://find-touchscreen.sh \
           file://busybox-cron \
           file://busybox-httpd \
           file://busybox-udhcpd \
           file://default.script \
           file://simple.script \
           file://hwclock.sh \
           file://mount.busybox \
           file://syslog \
           file://syslog-startup.conf \
           file://syslog.conf \
           file://busybox-syslog.default \
           file://mdev \
           file://mdev.conf \
           file://mdev-mount.sh \
           file://umount.busybox \
           file://busybox-syslog.service.in \
           file://busybox-klogd.service.in \
           file://fail_on_no_media.patch \
           file://run-ptest \
           file://inetd.conf \
           file://inetd \
           file://login-utilities.cfg \
           file://0001-build-system-Specify-nostldlib-when-linking-to-.o-fi.patch \
           file://recognize_connmand.patch \
           file://busybox-cross-menuconfig.patch \
           file://CVE-2014-9645_busybox_reject_module_names_with_slashes.patch \
           file://0006-busybox-1.22.1-udhcpc.patch \
           file://0100-busybox-1.22.1-ntp-log.patch \
           file://0101-busybox-1.22.1-arping-permissions.patch \
           file://0102-busybox-1.22.1-syslog.patch \
           file://0103-busybox-1.22.1-telnet.patch \
           file://0104-busybox-1.22.1-applets-permissions.patch \
           file://0105--busybox-1.22.1-syslog-counters.patch \
           file://0106--busybox-1.22.1-syslog-remotelog-localport.patch \
           file://0107--busybox-1.22.1-dhcpc-savexid.patch \
           file://defconfig \
"

# The default busybox config for all the machine should be mentioned in defconfig if it is common. If it is related to particular
#  target then update the respective .cfg config file. Ex: for pw2050.cfg for pw2050, sgx5150.cfg for sgx5150 and that will be appended below. 
SRC_URI_append_pw2050 = "file://pw2050.cfg"
SRC_URI_append_sgx5150 = "file://sgx5150.cfg"

SRC_URI[tarball.md5sum] = "337d1a15ab1cb1d4ed423168b1eb7d7e"
SRC_URI[tarball.sha256sum] = "ae0b029d0a9e4dd71a077a790840e496dd838998e4571b87b60fed7462b6678b"

EXTRA_OEMAKE += "V=1 ARCH=${TARGET_ARCH} CROSS_COMPILE=${TARGET_PREFIX} SKIP_STRIP=y"

do_install_ptest () {
        cp -r ${B}/testsuite ${D}${PTEST_PATH}/
        cp ${B}/.config      ${D}${PTEST_PATH}/
        ln -s /bin/busybox   ${D}${PTEST_PATH}/busybox
}
