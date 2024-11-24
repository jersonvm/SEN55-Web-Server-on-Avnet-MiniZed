# OS/Software used in this project:  
1. Ubuntu Linux v18.04.04.LTS 64-bit  
2. Vivado, Vitis and PetaLinux 2021.1  
3. GTKTerm or any serial port terminal  

# Hardware needed:  
1. Avnet MiniZed board  
2. Sensirion SEN55 all-in-one environmental sensor  
3. Host computer meeting or exceeding system requirements. See https://docs.amd.com/r/en-US/ug973-vivado-release-notes-install-license/System-Requirements & https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vivado/vivado-buy.html#tabs-413944f675-item-9598720e6a-tab  
4. USB thumb drive formatted as FAT/FAT32. At least 8GB in capacity.  

# Steps to build the OS  
1. Run the _build_petalinux_ script to create the petalinux build. This will also build the _minized_sbc_base_ Vivado project and the hardware platform needed for the petalinux build process. This is a 3-step process: initial build, rootfs configuration, and rebuild.  
2. After the initial build, the rootfs configuration menu will appear. Enable the _i2c-tools_ under the _Filesystem Packages --> base --> i2c-tools_.  
3. Save the changes to the rootfs configuration to continue to rebuilding the image with the rootfs changes.  

# Steps to flash the Petalinux image to the MiniZed board  
1. Attach the USB thumb drive to the host computer.  
2. Copy the _rootfs.wic_ located in _petalinux/projects/minized_sbc_base_2021_1/_ folder to the USB thumb drive or run the command _"cp petalinux/projects/minized_sbc_base_2021_1/rootfs.wic <thumb-drive-location>"_. When finished, remove the USB drive from the host computer.  
3. Apply power to the MiniZed board using the AUX PWR USB connector. Then, attach the MiniZed board to the host computer via USB JTAG/Serial port. Make sure that the boot select switch is set to JTAG mode before connecting to the host computer.  
4. Run the _flash_minimal_image_ script. This will flash the minimal Linux image that will be needed to program the larger Petalinux image (_rootfs.wic_ file that is copied to the USB drive) onto the eMMC of the MiniZed board later.  
5. Open the GTKTerm or any serial port terminal. Configure it by selecting the port assigned to the MiniZed board, and choosing the following: 115200 baud rate, no parity, 8 bits, 1 stopbit, and no flow control.  
6. Set the boot select switch of the MiniZed board to FLASH mode. Press the RESET button and watch the serial terminal as the minimal Petalinux image successfully boots up.  
7. Insert the USB thumb drive to the USB Host connector of the MiniZed board.  
8. Then, execute the Linux commands listed in the _"4- Boot from QSPI (u-boot) and EMMC (kernel & Ext4 rootfs)"_ section of _how_to_boot.txt_ located in _petalinux/projects/minized_sbc_base_2021_1/_ folder.  
9. Reboot the MiniZed board (execute Linux command: _shutdown -r now_) and watch the serial terminal as the full Petalinux image boots up.  

# Steps to connect to a WIFI network  
1. Boot the MiniZed board into Linux and make sure to be able to exuecute Linux commands onto it using a terminal.  
2. On the terminal, execute:  
   _vi /etc/wpa_supplicant.conf_  
3. Edit the wpa_supplicant.conf. Add the following lines:  
     _network={  
       key_mgmt=WPA-PSK  
       ssid="YOUR_SSID"  
       psk="YOUR_PASSWORD"  
       #scan_ssid=1 #uncomment if ssid is hidden  
     }_
4. To connect wo a WIFI network, execute:  
     _./wifi.sh_




