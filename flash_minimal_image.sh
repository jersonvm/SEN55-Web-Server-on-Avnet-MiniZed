#!/bin/bash

#Flash the minimal image onto the Avnet MiniZed
#Make sure that the board is connected to the host via the USB port for JTAG.
cd petalinux/projects/minized_sbc_base_2021_1/
sudo chmod u+x ./boot_qspi_INITRD_MINIMAL.sh
source /tools/Xilinx/Vitis/2021.1/settings64.sh
./boot_qspi_INITRD_MINIMAL.sh


