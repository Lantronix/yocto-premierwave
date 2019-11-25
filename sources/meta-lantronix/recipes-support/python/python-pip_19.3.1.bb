SUMMARY = "The PyPA recommended tool for installing Python packages"
HOMEPAGE = "https://pypi.python.org/pypi/pip"
SECTION = "devel/python"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=8ba06d529c955048e5ddd7c45459eb2e"

DEPENDS += "python python-setuptools-native"

SRC_URI[md5sum] = "1aaaf90fbafc50e7ba1e66ffceb00960"
SRC_URI[sha256sum] = "21207d76c1031e517668898a6b46a9fb1501c7a4710ef5dfd6a40ad9e6757ea7"

inherit pypi distutils

RDEPENDS_${PN} = "\
  python-compile \
  python-io \
  python-html \
  python-json \
  python-netserver \
  python-setuptools \
  python-unixadmin \
  python-xmlrpc \
  python-pickle \
"

BBCLASSEXTEND = "native nativesdk"
