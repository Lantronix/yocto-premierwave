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
    file://bluez_utils-devel-${PV}.tar.gz \
"
S = "${WORKDIR}/bluez_utils-devel-${PV}"

inherit autotools-brokensep pkgconfig systemd

EXTRA_OECONF = "\
  --enable-tools \
  --disable-cups \
  --enable-test \
  --enable-datafiles \
  ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '--enable-systemd', '--disable-systemd', d)} \
  --enable-experimental \
  --enable-library \
  --prefix=/ltrx_private/usr \
  --sysconfdir=/etc \
  --localstatedir=/ltrx_private/var \
  --disable-systemd \
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
sed -i '/$(AM_V_CCLD)$(LINK) $(tools_btgatt_server_OBJECTS) $(tools_btgatt_server_LDADD) $(LIBS)/c\\t$(AM_V_CCLD)$(LINK) $(tools_btgatt_server_OBJECTS) $(tools_btgatt_server_LDADD) $(LIBS) -lcrypto -lpthread' Makefile
sed -i '/lcrypto/a\lib/.libs/libbtgatt-server.so: $(tools_btgatt_server_lib_OBECTS)\n\t@rm -f lib/.libs/libbtgatt-server.so\n\t$(CCLD) -shared -o lib/.libs/libbtgatt-server.so $(tools_btgatt_server_lib_OBECTS)' Makefile
sed -i '/client\/gatt.c monitor\/uuid.h monitor\/uuid.c/c\\tclient/gatt.c monitor/uuid.h monitor/uuid.c\nam__client_ltrx_beacon_scanner_SOURCES_DIST = client/ltrx_beacon_scanner.c client/display.h \\\n\tclient/display.c client/agent.h client/agent.c \\\n\tclient/advertising.h client/advertising.c client/gatt.h \\\n\tclient/gatt.c monitor/uuid.h monitor/uuid.c' Makefile
sed -i '/client\/gatt.$(OBJEXT) monitor\/uuid.$(OBJEXT)/c\\tclient/gatt.$(OBJEXT) monitor/uuid.$(OBJEXT)\nam_client_ltrx_beacon_scanner_OBJECTS = client/ltrx_beacon_scanner.$(OBJEXT) \\\n\tclient/display.$(OBJEXT) client/agent.$(OBJEXT) \\\n\tclient/advertising.$(OBJEXT) \\\n\tclient/gatt.$(OBJEXT) monitor/uuid.$(OBJEXT)' Makefile
sed -i '/client_bluetoothctl_DEPENDENCIES =/c\client_ltrx_beacon_scanner_OBJECTS = $(am_client_ltrx_beacon_scanner_OBJECTS)\nclient_bluetoothctl_DEPENDENCIES = \\' Makefile
sed -i '/am__emulator_b1ee_SOURCES_DIST =/c\client_ltrx_beacon_scanner_DEPENDENCIES = \\\n\tgdbus/libgdbus-internal.la\ntools_ltrx_beacon_lib_OBJECTS = client/display.$(OBJEXT) \\\n\tclient/agent.$(OBJEXT) \\\n\tclient/advertising.$(OBJEXT) \\\n\tclient/gatt.$(OBJEXT) monitor/uuid.$(OBJEXT) \\\n\tgdbus/client.o gdbus/mainloop.o gdbus/object.o gdbus/polkit.o gdbus/watch.o\nam__emulator_b1ee_SOURCES_DIST = emulator/b1ee.c' Makefile
sed -i '/src\/attrib-server.h src\/attrib-server.c src\/gatt-database.h/c\\tsrc/attrib-server.h src/attrib-server.c src/gatt-database.h \\\n\tsrc/gatt_services.h src/ltrx_device_info_services.c src/ltrx_device_configuration_service.c src/gatt-database.c src/sdp-xml.h src/sdp-xml.c \\\n\tsrc/ltrx_le_advertising.h src/ltrx_le_advertising.c src/adapter.c src/profile.h src/profile.c src/service.h \\' Makefile
sed -i '/src\/bluetoothd-attrib-server.$(OBJEXT) \\/c\\tsrc/bluetoothd-attrib-server.$(OBJEXT) \\\n\tsrc/bluetoothd-ltrx_device_info_service.$(OBJEXT) \\\n\tsrc/bluetoothd-ltrx_device_configuration_service.$(OBJEXT) \\\n\tsrc/bluetoothd-ltrx_le_advertising.$(OBJEXT) \\' Makefile
sed -i '/$(attrib_gatttool_SOURCES) $(client_bluetoothctl_SOURCES) \\/c\\t$(attrib_gatttool_SOURCES) $(attrib_ltrx_gatttool_SOURCES) $(client_bluetoothctl_SOURCES) $(client_ltrx_beacon_scanner_SOURCES) \\' Makefile
sed -i '/$(am__client_bluetoothctl_SOURCES_DIST)/c\\t$(am__attrib_ltrx_gatttool_SOURCES_DIST) \\\n\t$(am__client_bluetoothctl_SOURCES_DIST) \\\n\t$(am__client_ltrx_beacon_scanner_SOURCES_DIST) \\' Makefile
sed -i '/src\/gatt-database.h src\/gatt-database.c \\/c\\t\t\tsrc/gatt-database.h src/gatt_services.h src/ltrx_device_info_services.c src/ltrx_device_configuration_service.c src/gatt-database.c \\' Makefile
sed -i '/src\/adapter.h src\/adapter.c \\/c\\t\t\tsrc/adapter.h src/ltrx_le_advertising.h src/ltrx_le_advertising.c src/adapter.c \\' Makefile
sed -i '/monitor_btmon_SOURCES =/c\client_ltrx_beacon_scanner_SOURCES = client/ltrx_beacon_scanner.c \\\n\t\t\t\t\tclient/display.h client/display.c \\\n\t\t\t\t\tclient/agent.h client/agent.c \\\n\t\t\t\t\tclient/advertising.h \\\n\t\t\t\t\tclient/advertising.c \\\n\t\t\t\t\tclient/gatt.h client/gatt.c \\\n\t\t\t\t\tmonitor/uuid.h monitor/uuid.c\n\nclient_ltrx_beacon_scanner_LDADD = $(client_bluetoothctl_LDADD)\n\nmonitor_btmon_SOURCES = monitor/main.c monitor/bt.h \\' Makefile
sed -i '/emulator\/b1ee.$(OBJEXT):/c\\nclient/ltrx_beacon_scanner$(EXEEXT): $(client_ltrx_beacon_scanner_OBJECTS) $(client_ltrx_beacon_scanner_DEPENDENCIES) $(EXTRA_client_ltrx_beacon_scanner_DEPENDENCIES) client/$(am__dirstamp)\n\t@rm -f client/ltrx_beacon_scanner$(EXEEXT)\n\t$(AM_V_CCLD)$(LINK) $(client_ltrx_beacon_scanner_OBJECTS) $(client_ltrx_beacon_scanner_LDADD) $(LIBS)\n\nlib/libltrx-beacon.so: $(tools_ltrx_beacon_lib_OBJECTS) client/bluetoothctl$(EXEEXT)\n\t@rm -f lib/libltrx-beacon.so\n\t$(CCLD) -shared -o lib/libltrx-beacon.so $(tools_ltrx_beacon_lib_OBJECTS) -lglib-2.0 -ldbus-1 -lreadline\n\nemulator/b1ee.$(OBJEXT): emulator/$(am__dirstamp) \\' Makefile
sed -i '/src\/bluetoothd-gatt-database.$(OBJEXT):/c\src/bluetoothd-ltrx_device_info_service.$(OBJEXT): src/$(am__dirstamp) \\\n\tsrc/$(DEPDIR)/$(am__dirstamp)\nsrc/bluetoothd-ltrx_device_configuration_service.$(OBJEXT): src/$(am__dirstamp) \\\n\tsrc/$(DEPDIR)/$(am__dirstamp)\nsrc/bluetoothd-ltrx_le_advertising.$(OBJEXT): src/$(am__dirstamp) \\\n\tsrc/$(DEPDIR)/$(am__dirstamp)\nsrc/bluetoothd-gatt-database.$(OBJEXT): src/$(am__dirstamp) \\' Makefile
sed -i '/(AM_V_CCLD)$(src_bluetoothd_LINK)/c\\t$(AM_V_CCLD)$(src_bluetoothd_LINK) $(src_bluetoothd_OBJECTS) $(src_bluetoothd_LDADD) $(LIBS) -lpthread' Makefile
sed -i '/include src\/$(DEPDIR)\/bluetoothd-gatt-client.Po/c\#include src/$(DEPDIR)/bluetoothd-gatt-client.Po\n#include src/$(DEPDIR)/bluetoothd-ltrx_device_info_service.Po\n#include src/$(DEPDIR)/bluetoothd-ltrx_device_configuration_service.Po\n#include src/$(DEPDIR)/bluetoothd-ltrx_le_advertising.Po' Makefile
sed -i '/src\/bluetoothd-gatt-database.o:/c\src/bluetoothd-ltrx_device_info_service.o: src/ltrx_device_info_service.c\n\t$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -MT src/bluetoothd-ltrx_device_info_service.o -MD -MP -MF src/$(DEPDIR)/bluetoothd-ltrx_device_info_service.Tpo -c -o src/bluetoothd-ltrx_device_info_service.o `test -f 'src/ltrx_device_info_service.c' || echo '$\(srcdir\)/'`src/ltrx_device_info_service.c\n\t$(AM_V_at)$(am__mv) src/$(DEPDIR)/bluetoothd-ltrx_device_info_service.Tpo src/$(DEPDIR)/bluetoothd-ltrx_device_info_service.Po\n#\t$(AM_V_CC)source='src/gatt-database.c' object='src/bluetoothd-gatt-database.o' libtool=no \\\n#\tDEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \\\n#\t$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -c -o src/bluetoothd-gatt-database.o `test -f 'src/gatt-database.c' || echo '$\(srcdir\)/'`src/gatt-database.c\n\nsrc/bluetoothd-ltrx_device_configuration_service.o: src/ltrx_device_configuration_service.c\n\t$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -MT src/bluetoothd-ltrx_device_configuration_service.o -MD -MP -MF src/$(DEPDIR)/bluetoothd-ltrx_device_configuration_service.Tpo -c -o src/bluetoothd-ltrx_device_configuration_service.o `test -f 'src/ltrx_device_configuration_service.c' || echo '$\(srcdir\)/'`src/ltrx_device_configuration_service.c\n\t$(AM_V_at)$(am__mv) src/$(DEPDIR)/bluetoothd-ltrx_device_configuration_service.Tpo src/$(DEPDIR)/bluetoothd-ltrx_device_configuration_service.Po\n#\t$(AM_V_CC)source='src/gatt-database.c' object='src/bluetoothd-gatt-database.o' libtool=no \\\n#\tDEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \\\n#\t$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -c -o src/bluetoothd-gatt-database.o `test -f 'src/gatt-database.c' || echo '$\(srcdir\)/'`src/gatt-database.c\n\nsrc/bluetoothd-gatt-database.o: src/gatt-database.c' Makefile
sed -i '/$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -MT src\/bluetoothd-gatt-database.o /c\\t$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) src/bluetoothd-ltrx_device_configuration_service.o src/bluetoothd-ltrx_device_info_service.o -MT src/bluetoothd-gatt-database.o src/bluetoothd-ltrx_device_configuration_service.o src/bluetoothd-ltrx_device_info_service.o -MD -MP -MF src/$(DEPDIR)/bluetoothd-gatt-database.Tpo -c -o src/bluetoothd-gatt-database.o `test -f 'src/gatt-database.c' || echo '$\(srcdir\)/'`src/gatt-database.c' Makefile
sed -i '/src\/bluetoothd-adapter.o:/c\src/bluetoothd-ltrx_le_advertising.o: src/ltrx_le_advertising.c\n\t$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -MT src/bluetoothd-ltrx_le_advertising.o -MD -MP -MF src/$(DEPDIR)/bluetoothd-ltrx_le_advertising.Tpo -c -o src/bluetoothd-ltrx_le_advertising.o `test -f 'src/ltrx_le_advertising.c' || echo '$\(srcdir\)/'`src/ltrx_le_advertising.c\n\t$(AM_V_at)$(am__mv) src/$(DEPDIR)/bluetoothd-ltrx_le_advertising.Tpo src/$(DEPDIR)/bluetoothd-ltrx_le_advertising.Po\n#\t$(AM_V_CC)source='src/ltrx_le_advertising.c' object='src/bluetoothd-ltrx_le_advertising.o' libtool=no \\\n#\tDEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \\\n#\t$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(src_bluetoothd_CFLAGS) $(CFLAGS) -c -o src/bluetoothd-ltrx_le_advertising.o `test -f 'src/ltrx_le_advertising.c' || echo '$\(srcdir\)/'`src/ltrx_le_advertising.c\n\nsrc/bluetoothd-adapter.o: src/adapter.c' Makefile
sed -i '/am__append_24 =/c\am__append_24 = attrib/gatttool attrib/ltrx_gatttool lib/libltrx-gattclient.so tools/btmgmt \\' Makefile
sed -i '/am__EXEEXT_6 =/c\am__EXEEXT_6 = attrib/gatttool$(EXEEXT) attrib/ltrx_gatttool$(EXEEXT) \\' Makefile
sed -i '/am__client_bluetoothctl_SOURCES_DIST =/c\am__attrib_ltrx_gatttool_SOURCES_DIST = attrib/ltrx_gatttool.c attrib/att.c \\\n\tattrib/gatt.c attrib/gattrib.c btio/btio.c attrib/gatttool.h \\\n\tattrib/ltrx_interactive.c attrib/utils.c src/log.c client/display.c \\\n\tclient/display.h\nam_attrib_ltrx_gatttool_OBJECTS = attrib/ltrx_gatttool.$(OBJEXT) \\\n\tattrib/att.$(OBJEXT) attrib/gatt.$(OBJEXT) \\\n\tattrib/gattrib.$(OBJEXT) btio/btio.$(OBJEXT) \\\n\tattrib/ltrx_interactive.$(OBJEXT) \\\n\tattrib/utils.$(OBJEXT) src/log.$(OBJEXT) \\\n\tclient/display.$(OBJEXT)\ntools_ltrx_gattclient_lib_OBJECTS = attrib/att.$(OBJEXT) \\\n\tattrib/gatt.$(OBJEXT) \\\n\tattrib/gattrib.$(OBJEXT) btio/btio.$(OBJEXT) \\\n\tattrib/ltrx_interactive.$(OBJEXT) attrib/utils.$(OBJEXT) \\\n\tsrc/shared/att.$(OBJEXT) src/shared/queue.$(OBJEXT) src/shared/util.$(OBJEXT) src/shared/crypto.$(OBJEXT) \\\n\tlib/hci.$(OBJEXT) lib/sdp.$(OBJEXT) lib/uuid.$(OBJEXT) lib/bluetooth.$(OBJEXT) \\\n\tsrc/shared/io-glib.$(OBJEXT) src/shared/timeout-glib.$(OBJEXT) src/log.$(OBJEXT) \\\n\tclient/display.$(OBJEXT) \\\n\tgdbus/client.o gdbus/mainloop.o gdbus/object.o gdbus/polkit.o gdbus/watch.o\nattrib_ltrx_gatttool_OBJECTS = $(am_attrib_ltrx_gatttool_OBJECTS)\nattrib_ltrx_gatttool_DEPENDENCIES =  \\\n\tlib/libbluetooth-internal.la \\\n\tsrc/libshared-glib.la\nam__client_bluetoothctl_SOURCES_DIST = client/main.c client/display.h \\' Makefile
sed -i '/attrib_gatttool_LDADD =/c\attrib_ltrx_gatttool_SOURCES = attrib/ltrx_gatttool.c attrib/att.c attrib/gatt.c \\\n\t\t\t\tattrib/gattrib.c btio/btio.c \\\n\t\t\t\tattrib/gatttool.h attrib/ltrx_interactive.c \\\n\t\t\t\tattrib/utils.c src/log.c client/display.c \\\n\t\t\t\tclient/display.h\n\nattrib_gatttool_LDADD = lib/libbluetooth-internal.la \\' Makefile
sed -i '/tools_obex_client_tool_SOURCES =/c\attrib_ltrx_gatttool_LDADD = $(attrib_gatttool_LDADD)\n\ntools_obex_client_tool_SOURCES = $(gobex_sources) $(btio_sources) \\' Makefile
sed -i '/attrib\/gatttool.$(OBJEXT):/c\attrib/ltrx_gatttool.$(OBJEXT): attrib/$(am__dirstamp) \\\n\tattrib/$(DEPDIR)/$(am__dirstamp)\nattrib/ltrx_interactive.$(OBJEXT): attrib/$(am__dirstamp) \\\n\tattrib/$(DEPDIR)/$(am__dirstamp)\nattrib/gatttool.$(OBJEXT): attrib/$(am__dirstamp) \\' Makefile
sed -i '/$(AM_V_CCLD)$(LINK) $(attrib_gatttool_OBJECTS) $(attrib_gatttool_LDADD) $(LIBS)/c\\t$(AM_V_CCLD)$(LINK) $(attrib_gatttool_OBJECTS) $(attrib_gatttool_LDADD) $(LIBS)\nattrib/ltrx_gatttool$(EXEEXT): $(attrib_ltrx_gatttool_OBJECTS) $(attrib_ltrx_gatttool_DEPENDENCIES) $(EXTRA_attrib_ltrx_gatttool_DEPENDENCIES) attrib/$(am__dirstamp)\n\t@rm -f attrib/ltrx_gatttool$(EXEEXT)\n\t$(AM_V_CCLD)$(LINK) $(attrib_ltrx_gatttool_OBJECTS) $(attrib_ltrx_gatttool_LDADD) $(LIBS)\n\nlib/libltrx-gattclient.so: $(tools_ltrx_gattclient_lib_OBJECTS) attrib/gatttool$(EXEEXT)\n\t@rm -f lib/libltrx-gattclient.so\n\t$(CCLD) -shared -o lib/libltrx-gattclient.so $(tools_ltrx_gattclient_lib_OBJECTS) -lglib-2.0 -ldbus-1 -lreadline' Makefile
sed -i '/include attrib\/$(DEPDIR)\/interactive.Po/c\include #attrib/$(DEPDIR)/interactive.Po\ninclude #attrib/$(DEPDIR)/ltrx_gatttool.Po\ninclude #attrib/$(DEPDIR)/ltrx_interactive.Po' Makefile
}

do_compile_append() {
	sed -i '/bin_PROGRAMS =/c\bin_PROGRAMS = tools/btgatt-server$(EXTEXT) tools/hciattach$(EXEEXT) tools/hciconfig$(EXEEXT) tools/hcitool$(EXEEXT) tools/bdaddr$(EXTEXT) tools/rfcomm$(EXTEXT) tools/sdptool$(EXTEXT) client/bluetoothctl$(EXTEXT) client/ltrx_beacon_scanner$(EXTEXT) attrib/ltrx_gatttool$(EXTEXT)' Makefile
	sed -i '/$(am__EXEEXT_4)/c\' Makefile
	sed -i '/am__EXEEXT_1 =/c\am__EXEEXT_1 = client/bluetoothctl$(EXEEXT) client/ltrx_beacon_scanner$(EXTEXT) lib/libltrx-beacon.so lib/.libs/libbtgatt-server.so' Makefile
	sed -i '/lib_LTLIBRARIES =/c\lib_LTLIBRARIES = $(am__append_2) lib/libltrx-beacon.so lib/libltrx-gattclient.so lib/.libs/libbtgatt-server.so lib/.libs/libbluetooth.so' Makefile
        sed -i '/am__append_12 =/c\am__append_12 = client/bluetoothctl client/ltrx_beacon_scanner lib/libltrx-beacon.so' Makefile
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
        install -D -m 0755 ${B}/lib/libltrx-beacon.so ${D}/usr/lib/libltrx-beacon.so
        install -D -m 0755 ${B}/lib/libltrx-gattclient.so ${D}/usr/lib/libltrx-gattclient.so
        install -d -m 0644 ${D}${includedir}/btio
        install -d -m 0644 ${D}${includedir}/lib
        install -d -m 0644 ${D}${includedir}/src/shared
        install -d -m 0644 ${D}${includedir}/dbus/
        
        install -D -m 0777 ${B}/src/bluetoothd ${D}/usr/lib/bluez5/bluetooth/
	cp ${S}/src/shared/att.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/att-types.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/gatt-db.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/gatt-server.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/mainloop.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/queue.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/timeout.h ${D}/usr/include/src/shared/
	cp ${S}/src/shared/util.h ${D}/usr/include/src/shared/
	cp ${S}/lib/uuid.h ${D}/usr/include/bluetooth/
        cp ${THISDIR}/bluez5/include/dbus/dbus.h ${D}/usr/include/dbus/
        cp ${THISDIR}/bluez5/include/advertising.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/agent.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/display.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/gatt.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/gdbus.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/uuid.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/util.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/att.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/gattrib.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/gatttool.h ${D}/usr/include/
        cp ${THISDIR}/bluez5/include/btio/btio.h ${D}/usr/include/btio/
        cp ${THISDIR}/bluez5/include/src/shared/crypto.h ${D}/usr/include/src/shared/
        cp ${THISDIR}/bluez5/include/src/shared/util.h ${D}/usr/include/src/shared/
        cp ${THISDIR}/bluez5/include/src/shared/gatt.h ${D}/usr/include/src/shared/
        cp ${THISDIR}/bluez5/include/lib/bluetooth.h ${D}/usr/include/lib/
        cp ${THISDIR}/bluez5/include/lib/hci.h ${D}/usr/include/lib/ 
        cp ${THISDIR}/bluez5/include/lib/hci_lib.h ${D}/usr/include/lib/ 
        cp ${THISDIR}/bluez5/include/lib/sdp.h ${D}/usr/include/lib/ 
        cp ${THISDIR}/bluez5/include/lib/uuid.h ${D}/usr/include/lib/ 
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
