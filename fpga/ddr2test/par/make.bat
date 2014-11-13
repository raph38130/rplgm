set XIL=C:\Documents and Settings\raph\Mes documents\xilinx\kcpsm3

"%XIL%\Assembler\kcpsm3.exe" %1 
rem "%XIL%\JTAG_loader\hex2svfsetup.exe"
"%XIL%\JTAG_loader\hex2svf.exe" %1.hex %1.svf 
"%XIL%\JTAG_loader\svf2xsvf.exe" -d -i %1.svf -o %1.xsvf 