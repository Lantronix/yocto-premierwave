LIC_FILES_CHKSUM = "file://LICENSE;md5=2d5025d4aa3495befef8f17206a5b0a1 \
                    file://avahi-common/address.h;endline=25;md5=b1d1d2cda1c07eb848ea7d6215712d9d \
                    file://avahi-core/dns.h;endline=23;md5=6fe82590b81aa0ddea5095b548e2fdcb \
                    file://avahi-daemon/main.c;endline=21;md5=9ee77368c5407af77caaef1b07285969 \
                    file://avahi-client/client.h;endline=23;md5=f4ac741a25c4f434039ba3e18c8674cf"

require avahi.inc


inherit python-dir pythonnative distro_features_check
ANY_OF_DISTRO_FEATURES = "${GTK2DISTROFEATURES}"

PACKAGECONFIG ??= "python"
PACKAGECONFIG[python] = "--enable-python,--disable-python,python-native python"

PR = "${INC_PR}.0"

SRC_URI[md5sum] = "2f22745b8f7368ad5a0a3fddac343f2d"
SRC_URI[sha256sum] = "8372719b24e2dd75de6f59bb1315e600db4fd092805bd1201ed0cb651a2dab48"

DEPENDS += "avahi gtk+ libglade"

AVAHI_GTK = "--enable-gtk --disable-gtk3"

S = "${WORKDIR}/avahi-${PV}"

PACKAGES = "${PN} ${PN}-utils ${PN}-dbg ${PN}-dev ${PN}-staticdev ${PN}-doc python-avahi avahi-discover avahi-discover-standalone"

FILES_${PN} = "${libdir}/libavahi-ui*.so.*"
FILES_${PN}-dbg += "${libdir}/.debug/libavah-ui*"
FILES_${PN}-dev += "${libdir}/libavahi-ui${SOLIBSDEV}"
FILES_${PN}-staticdev += "${libdir}/libavahi-ui.a"

FILES_${PN}-utils = "${bindir}/b* ${datadir}/applications/b*"

FILES_python-avahi = "${PYTHON_SITEPACKAGES_DIR}/avahi ${PYTHON_SITEPACKAGES_DIR}/avahi_discover"
FILES_avahi-discover = "${bindir}/avahi-discover \
                        ${datadir}/applications/avahi-discover.desktop \
                        ${datadir}/avahi/interfaces/avahi-discover*"
FILES_avahi-discover-standalone = "${bindir}/avahi-discover-standalone \
                                   ${datadir}/avahi/interfaces/avahi-discover.glade"

RDEPENDS_avahi-discover = "python-avahi python-pygtk"
RDEPENDS_python-avahi = "python-core python-dbus"



