PACKAGECONFIG_remove = "gnutls"
PACKAGECONFIG_append = " ssl"

EXTRA_OECONF := "${@oe_filter_out('--with-ca-bundle=${sysconfdir}/ssl/certs/ca-certificates.crt', '${EXTRA_OECONF}', d)}"
EXTRA_OECONF += " --without-ca-bundle "