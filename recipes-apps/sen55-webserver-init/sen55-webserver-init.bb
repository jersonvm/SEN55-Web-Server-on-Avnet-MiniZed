#
# This file is the sen55-webserver-init recipe.
#

SUMMARY = "Simple sen55-webserver-init application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://sen55-webserver-init \
	"

S = "${WORKDIR}"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit update-rc.d
INITSCRIPT_NAME = "sen55-webserver-init"
INITSCRIPT_PARAMS = "start 95 S ."

do_install() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/sen55-webserver-init ${D}${sysconfdir}/init.d/sen55-webserver-init
}

FILES_${PN} += "${sysconfdir}/*"
