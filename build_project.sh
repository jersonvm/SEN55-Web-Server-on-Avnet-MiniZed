#!/bin/bash

# Argument validation check
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <'MySsidName'> <'MyPassword'> #Keep the single quote <'> characters" 
    exit 1
fi

: << COMMENT
#Add Avnet git repositories as submodules
git submodule add https://github.com/avnet/bdf.git
git submodule add https://github.com/avnet/hdl.git
git submodule add https://github.com/avnet/petalinux.git

#Checkout needed branches
cd bdf
git checkout master
cd ../hdl
git checkout 2021.1
cd ../petalinux
git checkout 2021.1

#Clone hdl master to another folder
cd ..
git submodule add https://github.com/Avnet/hdl hdl2
cd hdl2
git checkout master
cd ..

git submodule add https://github.com/Sensirion/embedded-i2c-sen5x
cd embedded-i2c-sen5x
git checkout master
cd ..
COMMENT

git submodule update --init --recursive

YELLOW='\033[01;33m'
NONE='\033[00m'
BOLD='\033[1m'

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Updating minized_sensor_fusion.tcl${NONE}"
sed -i.bak 's/axi_iic:2.0/axi_iic:2.1/g' hdl2/Scripts/ProjectScripts/minized_sensor_fusion.tcl
cd hdl2/Scripts

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Building minized_sensor_fusion Vivado project${NONE}"
vivado -mode batch -source make_minized_sensor_fusion.tcl
cd ../..

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Exporting minized_sensor_fusion XSA${NONE}"
vivado -mode batch -source export_hardware.tcl

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Creating initial Petalinux build${NONE}"
cd petalinux/scripts
source /tools/Xilinx/Vivado/2021.1/settings64.sh
./make_minized_sbc_base.sh

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Configuring Petalinux - get hw description${NONE}"
cd ../projects/minized_sbc_base_2021_1
echo E | petalinux-config --get-hw-description=../../../hdl2/Projects/minized_sensor_fusion/MINIZED_2018_2

cd petalinux/projects/minized_sbc_base_2021_1

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Configuring rootfs - enable i2c tools${NONE}"
sed -i.bak 's/# CONFIG_i2c-tools is not set/CONFIG_i2c-tools=y/g' project-spec/configs/rootfs_config

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Copying recipe apps containing sen55 webserver source code${NONE}"
cp -r ../../../recipes-apps project-spec/meta-avnet

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Configuring rootfs - add sen55 webserver recipe apps in recipes-core${NONE}"
sed -i.bak2 -e '$a\\nIMAGE_INSTALL_append_minized-sbc = "\\ \n        sen55-webserver-init \\ \n        sen55-webserver \\ \n"' project-spec/meta-avnet/recipes-core/images/avnet-image-full.inc

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Configuring rootfs - remove conflicted apps in recipes-core${NONE}"
sed -i.bak1 -e '$a\\nIMAGE_INSTALL_remove_minized-sbc = "\\ \n        python-webserver-init \\ \n        python-webserver \\ \n"' project-spec/meta-avnet/recipes-core/images/avnet-image-full.inc

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Configuring wpa_supplicant${NONE}"
SEDARG="network={\nkey_mgmt=WPA-PSK\nssid=\"MySsidName\"\npsk=\"MyPassword\"\nscan_ssid=1\n}"
sed -i.bak -e '$a\\nnetwork' project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane
sed -i "s/network/$SEDARG/" project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane
sed -i "s/MySsidName/$1/" project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane
sed -i "s/MyPassword/$2/" project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Rebuilding final Petalinux image${NONE}"
./rebuild_minized_sbc_base.sh

echo ""
echo -e "${BOLD}${YELLOW}BUILD STEP:${NONE}${YELLOW} Cleaning wpa_supplicant${NONE}"
rm -f project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane
cp project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane.bak project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane
rm -f project-spec/meta-avnet/recipes-connectivity/wpa-supplicant/files/wpa_supplicant.conf-sane.bak

cd ../../..
rm -rf vivado*
rm -rf .Xil

