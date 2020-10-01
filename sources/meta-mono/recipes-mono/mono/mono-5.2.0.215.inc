SRC_URI[md5sum] = "fbb20660fd70ffd14a8b6a3b7ddcbbb5"
SRC_URI[sha256sum] = "8f0cebd3f7b03f68b9bd015706da9c713ed968004612f1ef8350993d8fe850ea"

S = "${WORKDIR}/mono-${PV}"

EXTRA_OECONF = "--disable-btls --disable-system-aot --enable-static--with-ikvm-native=no --with-mcs-docs=no --enable-minimal=profiler,debug --disable-boehm"
