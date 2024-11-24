# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/training/SEN55-Web-Server-on-Avnet-MiniZed/sen55/Minized-Petalinux/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/training/SEN55-Web-Server-on-Avnet-MiniZed/sen55/Minized-Petalinux/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Minized-Petalinux}\
-hw {/home/training/AvnetTTC/PetaLinux/2021_1/minized_sensor_fusion_wrapper.xsa}\
-proc {ps7_cortexa9} -os {linux} -no-boot-bsp -out {/home/training/SEN55-Web-Server-on-Avnet-MiniZed/sen55}

platform write
platform active {Minized-Petalinux}
domain config -bootmode {qspi}
platform write
platform generate
platform clean
platform active {Minized-Petalinux}
