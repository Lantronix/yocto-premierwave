inherit pypi setuptools
require python-schedule.inc

RDEPENDS_${PN} += " \
    ${PYTHON_PN}-subprocess \
    ${PYTHON_PN}-zlib \
"
