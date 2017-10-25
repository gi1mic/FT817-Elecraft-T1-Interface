# FT817-Elecraft-T1-Interface by GI1MIC


A Low cost FT817 to Elecraft T1 ATU cable

![alt text](https://github.com/gi1mic/FT817-Elecraft-T1-Interface/blob/master/Photos/IMG_8588.jpg "FT817 to Elecraft T1 ATU cable")

I recently became the proud owner of an Elecraft T1 ATU which I planned to use with my trusty FT817. After reading the manual I was encouraged to get the optional FT817 interface cable until I found out it cost over £70 plus P&P here in the UK.

Part of the reason the cable costs so much due to the fact it has a small microprocessor in it. This micro reads the analogue BAND_DATA pin on the ACC port converts it into a four-bit digital word which is sent to the T1.
Given the high cost I decided to make my own. A quick search of the internet turned up a project by Matthew Robinson (VK6MR) where he had done something already to interface the T1 to the Flex-1500 SDR (See http://blog.zensunni.org/). Rather than re-invent the wheel I used some of his code to reduce my development time – thank you Matthew!

I used an 5V Arduino Pro Micro since:
- It is cheap plus I had one to hand
- It is relatively small
- It has a built in USB port making it easy to program. Just configure the Arduino IDE for a Leonardo board and plug the board in using a USB cable
- Its ADC covers the required voltage range of the BAND_DATA pin
- The RAW power input is rated up to 16V so it can be powered directly from the 13.8V output on the ACC connector
- It has diode protection allowing the board to be power from either the USB port or the RAW input without frying the USB port if the RAW input goes above 5V.
- It can be put into sleep mode to reduce power consumption 

# Parts list
* 5V Arduino pro micro (I think these are always 16Mhz)
* 3.5mm Stereo jack plug
* Just about any NPN transistor
* A 1K resistor (value not that critical)
* A mini DIN plug to connect to the ACC connector on the back of the FT817
* Some shielded two-way cable (GND/Power/BAND_DATA for the ACC and TIP/Ring/GND for the 3.mm jack) 
* Some heat shrink

# Assembly
I just assembled the parts to the rear of the Arduino (See photos).

![alt text](https://github.com/gi1mic/FT817-Elecraft-T1-Interface/blob/master/Photos/IMG_8601.jpg "Rear of the board")

|ACC Port | Arduino |
| ------ | ------ |
|13.8V    | RAW  |
|BAND_DATA| A3   |
|GND      | GND  |

|T1 3.5MM Plug | Arduino |
| ------ | ------ |
|TIP           | D9      |
|RING          | Collector of NPN. Base of NPN goes to Arduino D8 via a 1K resistor |
|GND           | GND     |  

![alt text](https://github.com/gi1mic/FT817-Elecraft-T1-Interface/blob/master/Photos/interface.jpg "Arduino to T1 interface")


The final cable assembly was held in place to the board with hot glue before being wrapped in heat shrink. In hind sight I should have also wired up the RX and TX lines from the ACC port to the Arduino to allow the board to also act as a CAT interface or to read/set options on the FT817 by the Arduino.

# Power Consumption
The documentation for the official interface cable states it draws 0.06 mA with the rig off and 0.3 mA when the rig is on.  This circuit draws about 3mA but bursts to much higher than that every two seconds for a fraction of a second as it samples the BAND_DATA pin. 

I suspect the official cable draws so little power that is powered via the TX output pin. Unfortunately, that can’t be down with an Arduino. Still this build is 1/7th the cost of an official cable.

It is possible to reduce the power consumption of the board a little bit more by removing the power LED and changing the sleep time period in the code. 
