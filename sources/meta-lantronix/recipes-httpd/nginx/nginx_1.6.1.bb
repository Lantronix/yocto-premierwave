SUMMARY = "HTTP and reverse proxy server"

DESCRIPTION = "Nginx is a web server and a reverse proxy server for \
HTTP, SMTP, POP3 and IMAP protocols, with a strong focus on high  \
concurrency, performance and low memory usage."

HOMEPAGE = "http://nginx.org/"
LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=9d3b27bad611f5204a84ba6a572698e1"
SECTION = "net"

DEPENDS = "libpcre openssl"

# upstream: http://nginx.org/download/nginx-${PV}.tar.gz

S = "${WORKDIR}/nginx-develop-1.6.1/"

SRC_URI = " \
	file://nginx-develop-1.6.1.tar.gz \
    file://nginx.conf \
	file://nginx.init \
	file://nginx-volatile.conf \
	file://nginx.service \
	file://nginx-013-login_audit.patch \
"
SRC_URI[md5sum] = "2562320f1535e3e31d165e337ae94f21"
SRC_URI[sha256sum] = "48e2787a6b245277e37cb7c5a31b1549a0bbacf288aa4731baacf9eaacdb481b"

inherit update-rc.d useradd

CFLAGS_append = " -fPIE -pie"
CXXFLAGS_append = " -fPIE -pie"

EXTRA_OECONF = "--with-ipv6"

do_configure[noexec] = "1"

EXTRA_OEMAKE = "DESTDIR=${D} \
                CFLAGS='${CFLAGS}' \
                LDFLAGS='${LDFLAGS}' \
                STAGING_DIR='${STAGING_DIR}' \
                STRIPPROG='${STRIP}' \
               "

do_compile() {
    oe_runmake 'DESTDIR=${D}'
}

INSANE_SKIP_${PN} = "installed-vs-shipped "

do_install () {
	oe_runmake 'DESTDIR=${D}' install
    install -m 0755 -d ${D}/bin/
    install -m 0755 ${S}/objs/nginx ${D}/bin/nginx
	rm -fr ${D}${localstatedir}/run ${D}/run
	if ${@base_contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
		install -d ${D}${sysconfdir}/tmpfiles.d
		echo "d /run/${BPN} - - - -" \
		     > ${D}${sysconfdir}/tmpfiles.d/${BPN}.conf
	fi
	install -d ${D}${sysconfdir}/${BPN}
	ln -snf ${localstatedir}/run/${BPN} ${D}${sysconfdir}/${BPN}/run
	install -d ${D}${localstatedir}/www/localhost
	mv ${D}/usr/local/nginx/html ${D}${localstatedir}/www/localhost/
	chown www:www-data -R ${D}${localstatedir}

	install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/nginx.init ${D}${sysconfdir}/init.d/nginx
	sed -i 's,/usr/sbin/,${sbindir}/,g' ${D}${sysconfdir}/init.d/nginx
	sed -i 's,/etc/,${sysconfdir}/,g'  ${D}${sysconfdir}/init.d/nginx

	#install -d ${D}${sysconfdir}/nginx
	install -m 0644 ${WORKDIR}/nginx.conf ${D}${sysconfdir}/nginx.conf
	sed -i 's,/var/,${localstatedir}/,g' ${D}${sysconfdir}/nginx.conf
	install -d ${D}${sysconfdir}/sites-enabled

	install -d ${D}${sysconfdir}/default/volatiles
	install -m 0644 ${WORKDIR}/nginx-volatile.conf ${D}${sysconfdir}/default/volatiles/99_nginx
	sed -i 's,/var/,${localstatedir}/,g' ${D}${sysconfdir}/default/volatiles/99_nginx

        if ${@bb.utils.contains('DISTRO_FEATURES','systemd','true','false',d)};then
            install -d ${D}${systemd_unitdir}/system
            install -m 0644 ${WORKDIR}/nginx.service ${D}${systemd_unitdir}/system/
            sed -i -e 's,@SYSCONFDIR@,${sysconfdir},g' \
                    -e 's,@LOCALSTATEDIR@,${localstatedir},g' \
                    -e 's,@BASEBINDIR@,${base_bindir},g' \
                    ${D}${systemd_unitdir}/system/nginx.service
        fi
}

pkg_postinst_${PN} () {
	if [ -z "$D" ]; then
		if type systemd-tmpfiles >/dev/null; then
			systemd-tmpfiles --create
		elif [ -e ${sysconfdir}/init.d/populate-volatile.sh ]; then
			${sysconfdir}/init.d/populate-volatile.sh update
		fi
	fi
}

FILES_${PN} += "${localstatedir}/ \
                ${systemd_unitdir}/system/nginx.service \
                "

CONFFILES_${PN} = "${sysconfdir}/nginx.conf \
		${sysconfdir}/fastcgi.conf\
		${sysconfdir}/fastcgi_params \
		${sysconfdir}/koi-utf \
		${sysconfdir}/koi-win \
		${sysconfdir}/mime.types \
		${sysconfdir}/scgi_params \
		${sysconfdir}/uwsgi_params \
		${sysconfdir}/win-utf \
"

INITSCRIPT_NAME = "nginx"
INITSCRIPT_PARAMS = "defaults 92 20"

USERADD_PACKAGES = "${PN}"
USERADD_PARAM_${PN} = " \
    --system --no-create-home \
    --home ${localstatedir}/www/localhost \
    --groups www-data \
    --user-group www"
