SUMMARY = "Linux Bluetooth Stack Userland V5"
DESCRIPTION = "Linux Bluetooth stack V5 userland components.  These include a system configurations, daemons, tools and system libraries."
HOMEPAGE = "http://www.bluez.org"
SECTION = "libs"
LICENSE = "GPLv2+ & LGPLv2.1+"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e \
                    file://COPYING.LIB;md5=fb504b67c50331fc78734fed90fb0e09 \
                    file://src/main.c;beginline=1;endline=24;md5=9bc54b93cd7e17bf03f52513f39f926e"
DEPENDS = "udev libusb dbus-glib glib-2.0 libcheck zeromq readline"
PROVIDES += "bluez-hcidump"
RPROVIDES_${PN} += "bluez-hcidump"

RCONFLICTS_${PN} = "bluez4"

PACKAGECONFIG ??= "obex-profiles"
PACKAGECONFIG[obex-profiles] = "--enable-obex,--disable-obex,libical"
PACKAGECONFIG[experimental] = "--enable-experimental"

SRC_URI = "\
    ${KERNELORG_MIRROR}/linux/bluetooth/bluez-${PV}.tar.xz \
    file://btgatt-server.patch \
    ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '', 'file://0001-Allow-using-obexd-without-systemd-in-the-user-sessio.patch', d)} \
"
S = "${WORKDIR}/bluez-${PV}"

inherit autotools-brokensep pkgconfig systemd

EXTRA_OECONF = "\
  --enable-tools \
  --disable-cups \
  --enable-test \
  --enable-datafiles \
  ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '--enable-systemd', '--disable-systemd', d)} \
  --enable-experimental \
  --enable-library \
"

# bluez5 builds a large number of useful utilities but does not
# install them.  Specify which ones we want put into ${PN}-noinst-tools.
NOINST_TOOLS_READLINE ??= ""
NOINST_TOOLS_EXPERIMENTAL ??= ""
NOINST_TOOLS = " \
    ${NOINST_TOOLS_READLINE} \
    ${@bb.utils.contains('PACKAGECONFIG', 'experimental', '${NOINST_TOOLS_EXPERIMENTAL}', '', d)} \
"

do_configure_append() {
sed -i '/tools\/btgatt-server tools\/test-runner /c\\ttools/btgatt-server lib/.libs/libbtgatt-server.so tools/test-runner \\' Makefile
sed -i '/$(am_tools_btgatt_server_OBJECTS)/a\tools_btgatt_server_lib_OBECTS = src/shared/.libs/att.o src/shared/.libs/crypto.o src/shared/.libs/gatt-server.o src/shared/.libs/io-mainloop.o src/shared/.libs/mainloop.o src/shared/.libs/timeout-mainloop.o lib/.libs/uuid.o lib/.libs/bluetooth.o src/shared/.libs/gatt-db.o lib/.libs/hci.o src/shared/.libs/queue.o src/shared/.libs/util.o\' Makefile
sed -i '/am__EXEEXT_1 =/c\am__EXEEXT_1 = client/bluetoothctl$(EXEEXT)	tools/btgatt-server$(EXTEXT)' Makefile
sed -i '/$(AM_V_CCLD)$(LINK) $(tools_btgatt_server_OBJECTS) $(tools_btgatt_server_LDADD) $(LIBS)/c\\t$(AM_V_CCLD)$(LINK) $(tools_btgatt_server_OBJECTS) $(tools_btgatt_server_LDADD) $(LIBS) -lcrypto ' Makefile
sed -i '/lcrypto/a\lib/.libs/libbtgatt-server.so: $(tools_btgatt_server_lib_OBECTS)\n\t@rm -f lib/.libs/libbtgatt-server.so\n\t$(CCLD) -shared -o lib/.libs/libbtgatt-server.so $(tools_btgatt_server_lib_OBECTS)' Makefile
}

do_compile_append() {
	sed -i '/bin_PROGRAMS =/c\bin_PROGRAMS = tools/btgatt-server$(EXTEXT) tools/hciattach$(EXEEXT) tools/hciconfig$(EXEEXT) tools/hcitool$(EXEEXT) tools/bdaddr$(EXTEXT)' Makefile
	sed -i '/$(am__EXEEXT_4)/c\' Makefile
	sed -i '/am__EXEEXT_1 =/c\am__EXEEXT_1 = client/bluetoothctl$(EXEEXT)	tools/btgatt-server$(EXTEXT)	lib/.libs/libbtgatt-server.so' Makefile
	sed -i '/lib_LTLIBRARIES =/c\lib_LTLIBRARIES = lib/.libs/libbtgatt-server.so lib/.libs/libbluetooth.so' Makefile
}

do_install_append() {
	install -d ${D}${sysconfdir}/bluetooth/
	if [ -f ${S}/profiles/audio/audio.conf ]; then
	    install -m 0644 ${S}/profiles/audio/audio.conf ${D}/${sysconfdir}/bluetooth/
	fi
	if [ -f ${S}/profiles/network/network.conf ]; then
	    install -m 0644 ${S}/profiles/network/network.conf ${D}/${sysconfdir}/bluetooth/
	fi
	if [ -f ${S}/profiles/input/input.conf ]; then
	    install -m 0644 ${S}/profiles/input/input.conf ${D}/${sysconfdir}/bluetooth/
	fi

	install -m 0644 ${S}/src/bluetooth.conf ${D}/${sysconfdir}/dbus-1/system.d/

	# Install desired tools that upstream leaves in build area
        for f in ${NOINST_TOOLS} ; do
	    install -m 755 ${B}/$f ${D}/${bindir}
	done
        install -D -m 0755 ${B}/lib/.libs/libbtgatt-server.so ${D}/usr/lib/libbtgatt-server.so
        install -D -m 0755 ${B}/lib/.libs/libbluetooth.so ${D}/usr/lib/libbluetooth.so
        install -d -m 0644 ${D}${includedir}/src/shared
	cp ${S}/src/shared/att.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/att-types.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/gatt-db.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/gatt-server.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/mainloop.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/queue.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/timeout.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/util.h ${D}/usr/include/src/shared/
	cp ${S}/lib/uuid.h ${D}/usr/include/bluetooth/
}

FILES_${PN}-dev = "${includedir}"
FILES_${PN} += "${B}/lib/.libs/libbtgatt-server.so* ${libdir}/libbtgatt-server.so"
FILES_${PN} += "${B}/lib/.libs/libbluetooth.so* ${libdir}/libbluetooth.so"
FILES_${PN} += "${libdir}/*"

ALLOW_EMPTY_libasound-module-bluez = "1"
PACKAGES =+ "libasound-module-bluez ${PN}-testtools ${PN}-obex ${PN}-noinst-tools"

FILES_libasound-module-bluez = "${libdir}/alsa-lib/lib*.so ${datadir}/alsa"
FILES_${PN} += "${libdir}/bluetooth/plugins/*.so ${base_libdir}/udev/ ${nonarch_base_libdir}/udev/ ${systemd_unitdir}/ ${datadir}/dbus-1"
FILES_${PN}-dev += "\
  ${libdir}/bluetooth/plugins/*.la \
  ${libdir}/alsa-lib/*.la \
"


FILES_${PN}-obex = "${libexecdir}/bluetooth/obexd \
                    ${exec_prefix}/lib/systemd/user/obex.service \
                    ${datadir}/dbus-1/services/org.bluez.obex.service \
                   "
SYSTEMD_SERVICE_${PN}-obex = "obex.service"

FILES_${PN}-testtools = "${libdir}/bluez/test/*"

def get_noinst_tools_paths (d, bb, tools):
    s = list()
    bindir = d.getVar("bindir", True)
    for bdp in tools.split():
        f = os.path.basename(bdp)
        s.append("%s/%s" % (bindir, f))
    return "\n".join(s)

FILES_${PN}-noinst-tools = "${@get_noinst_tools_paths(d, bb, d.getVar('NOINST_TOOLS', True))}"

FILES_${PN}-dbg += "\
  ${libexecdir}/bluetooth/.debug \
  ${libdir}/bluetooth/plugins/.debug \
  ${libdir}/*/.debug \
  */udev/.debug \
  */*/udev/.debug \
  "

RDEPENDS_${PN}-testtools += "python python-dbus python-pygobject"

SYSTEMD_SERVICE_${PN} = "bluetooth.service"

EXCLUDE_FROM_WORLD = "1"
