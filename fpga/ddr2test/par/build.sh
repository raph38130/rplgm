../../Picoasm/picoasm -i control.psm -t ROM_form.vhd
grep [0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f] control.log -o > control.hex
wine ../../kcpsm3/JTAG_loader/hex2svf.exe control.hex control.svf
wine ../../kcpsm3/JTAG_loader/svf2xsvf.exe -d -i control.svf -o control.xsvf
