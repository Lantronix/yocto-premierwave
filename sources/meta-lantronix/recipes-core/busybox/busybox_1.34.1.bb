require busybox.inc

PR = "r32"

SRC_URI = "http://www.busybox.net/downloads/busybox-${PV}.tar.bz2;name=tarball \
           file://get_header_tar.patch \
           file://busybox-appletlib-dependency.patch \
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
           file://recognize_connmand.patch \
           file://CVE-2014-9645_busybox_reject_module_names_with_slashes.patch \
           file://0100-busybox-1.34.1-ntp-log.patch \
           file://0101-busybox-1.34.1-arping-permissions.patch \
           file://0102-busybox-1.34.1-syslog.patch \
           file://0103-busybox-1.34.1-telnet.patch \
           file://0104-busybox-1.34.1-applets-permissions.patch \
           file://0105-busybox-1.34.1-dhcpc-savexid.patch \
           file://defconfig \
"

# The default busybox config for all the machine should be mentioned in defconfig if it is common. If it is related to particular
#  target then update the respective .cfg config file. Ex: for pw2050.cfg for pw2050, sgx5150.cfg for sgx5150 and that will be appended below. 
SRC_URI_append_pw2050 = "file://pw2050.cfg"
SRC_URI_append_sgx5150 = "file://sgx5150.cfg"

SRC_URI[tarball.md5sum] = "5ad7368a73d12b8c4f8881bf7afb3d82"
SRC_URI[tarball.sha256sum] = "415fbd89e5344c96acf449d94a6f956dbed62e18e835fc83e064db33a34bd549"

EXTRA_OEMAKE += "V=1 ARCH=${TARGET_ARCH} CROSS_COMPILE=${TARGET_PREFIX} SKIP_STRIP=y"

do_install_ptest () {
        cp -r ${B}/testsuite ${D}${PTEST_PATH}/
        cp ${B}/.config      ${D}${PTEST_PATH}/
        ln -s /bin/busybox   ${D}${PTEST_PATH}/busybox
}
