FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://0001-wwan-fix-provision-of-IPv6-addresses.patch \
	file://NetworkManager.conf \
	file://cellular.generic \
"

# NOTE: the patch is only needed with NetworkManager 1.36, in 1.38 and above it
#       is already there

PACKAGECONFIG:append = " modemmanager"
PACKAGECONFIG:remove = " dnsmasq"

do_install:append() {
    install -D -m 0600 ${WORKDIR}/NetworkManager.conf ${D}${sysconfdir}/NetworkManager/
    install -D -m 0600 ${WORKDIR}/cellular.generic    ${D}${sysconfdir}/NetworkManager/system-connections/cellular.generic
}

ALTERNATIVE_PRIORITY[resolv-conf] = "10"
