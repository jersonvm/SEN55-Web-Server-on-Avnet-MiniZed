#!/bin/bash

#Clone submodules
git submodule update --init --recursive

#Build the initial Petalinux image for the MiniZed
cd petalinux/scripts
source /tools/Xilinx/Vivado/2021.1/settings64.sh
./make_minized_sbc_base.sh

#Configure Petalinux
cd ../projects/minized_sbc_base_2021_1
#petalinux-config --get-hw-description=~/SEN55-Web-Server-on-Avnet-MiniZed/hdl/projects/minized_sbc_base_2021_1
petalinux-config -c rootfs
#petalinux-config -c kernel

#Rebuild the final Petalinux image
./rebuild_minized_sbc_base.sh


