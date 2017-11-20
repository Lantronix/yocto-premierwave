require dropbear.inc

SRC_URI[md5sum] = "7066bb9a2da708f3ed06314fdc9c47fd"
SRC_URI[sha256sum] = "595992de432ba586a0e7e191bbb1ad587727678bb3e345b018c395b8c55b57ae"

SRC_URI += "file://dropbear-2014.63-001-maxsessions.patch \
            file://dropbear-2014.63-002-escapesequence.patch \
            file://dropbear-2014.63-003-quietmode.patch \
            file://dropbear-2014.63-004-password.patch \
            file://dropbear-2014.63-005-homedir.patch \
            file://dropbear-2014.63-006-no_timerlist_entropy.patch \
"
