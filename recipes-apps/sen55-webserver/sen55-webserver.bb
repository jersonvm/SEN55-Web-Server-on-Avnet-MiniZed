#
# This file is the sen55 recipe.
#

SUMMARY = "Simple sen55 application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Packages
RDEPENDS_${PN} += "\
  python3-core \
  python-core \
    "

SRC_URI = "file://main.c \
	   file://sensirion_config.h \
	   file://sen5x_i2c.h \
	   file://sen5x_i2c.c \
	   file://sensirion_common.h \
	   file://sensirion_common.c \
	   file://sensirion_i2c.h \
	   file://sensirion_i2c.c \
	   file://sensirion_i2c_hal.h \
	   file://sensirion_i2c_hal.c \
	   file://xplatform_to_app.h \
	   file://xplatform_to_app.c \
	   file://server.py \
	   file://main.css \
	   file://launch_server.sh \
	   file://stop_server.sh \
	   file://Makefile \
		  "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/sen55-webserver*"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root/sen55-webserver
	     install -m 0755 sen55 ${D}/home/root/sen55-webserver
	     install -m 0755 ${S}/server.py ${D}/home/root/sen55-webserver		
	     install -m 0755 ${S}/main.css ${D}/home/root/sen55-webserver
	     install -m 0755 ${S}/launch_server.sh ${D}/home/root/sen55-webserver		
	     install -m 0755 ${S}/stop_server.sh ${D}/home/root/sen55-webserver
}
