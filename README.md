# Slot car speed controller for two 1:24 cars using a PIC16F18313

This was made as a plug-in replacement upgrade for the horrible capacitor popping controller found in the Hot Wheels slot car sets from Mattel.

![Hot Wheels](/images/hotwheels.jpg)

## Features:  
* It doesn't break if you short circuit a track.  
* Actual speed control, you can now go smoothly from still to full speed, instead of "still, jump to 70%, smooth to 100%".

Two self resetting poly-fuses limit the current to 500mA per car, at 12V this should be enough for most 1:24 cars. I use 9V for the Hot Wheels circuit.

![Schematic](/images/schematic.png)

![PCB bottom](/images/pcb_bottom.png)

![PCB top](/images/pcb_top.png)

![Pinout](/images/pinout.png)
