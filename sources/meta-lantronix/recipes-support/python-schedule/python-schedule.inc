SUMMARY = "The Swiss Army knife of Python Schedule"
DESCRIPTION = "\
Werkzeug started as simple collection of various utilities for WSGI \
applications and has become one of the most advanced WSGI utility modules. \
It includes a powerful debugger, full featured request and response objects, \
HTTP utilities to handle entity tags, cache control headers, HTTP dates, \
cookie handling, file uploads, a powerful URL routing system and a bunch \
of community contributed addon modules."
HOMEPAGE = "https://pypi.org/project/schedule/"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=6400f153491d45ea3459761627ca24b2"

SRC_URI[md5sum] = "53fb2e550b2539264eeceb5d85e82759"
SRC_URI[sha256sum] = "1003a07c2dce12828c25a03a611a7371cedfa956e5f1b4abc32bcc94eb5a335b"

PYPI_PACKAGE = "schedule"
inherit pypi
CLEANBROKEN = "1"
