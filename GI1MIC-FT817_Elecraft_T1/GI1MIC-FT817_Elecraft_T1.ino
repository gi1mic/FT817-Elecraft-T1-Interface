/******************************************************
*  Elecraft T1 FT817 Interface                        *
*                                                     *
*  This Arduino sketch allows the FT817 to control    *
*  an Elecraft T1 ATU via the T1's remote control     *
*  interface.                                         *
*                                                     *
*  Copyright (c) Gareth - GI1MIC, 2017                *
*                                                     *
*                                                     *
*                                                     *
*                                                     *
*  Based on stuff from Matthew Robinson - VK6MR, 2011 *
*  See http://blog.zensunni.org/                      *
*                                                     * 
*  This code is released under a Creative Commons     *
*  Attribution-NonCommercial-ShareAlike 3.0           *
*  Unported (CC BY-NC-SA 3.0) license.                *
*                                                     *
*  http://creativecommons.org/licenses/by-nc-sa/3.0/  *
*                                                     *
******************************************************/

#include <Adafruit_SleepyDog.h>

// Comment out the following to enable degugging and 
// disable power saving (which clashes with the USB interface)
// #define debug


struct bandTable {
  const int      band;
  const String   bandText;
  const int      adcValue;
};
/*                BandCode, Band, adcValue */
struct bandTable bandTableList[] = {
                      {1,  "160m",  67},
                      {2,  "80m", 145},
                      {3,  "40m", 215},
                      {4,  "30m", 277},
                      {5,  "20m", 347},
                      {6,  "17m", 425},
                      {7,  "15m", 496},
                      {8,  "12m", 556},
                      {9,  "10m", 627},
                      {10, "50m", 707}
//                      {11, "VHF", 778},     // T1 can't do this band
//                      {12, "UHF", 841}      // T1 can't do this band
    };

const int analogInPin = A3;  // Analog input pin to FT817 ACC band data pin
int TUNE = 8;                // Ring of Elecraft T1 3.5mm jack via 1K and NPN transistor
int DATA = 9;                // Tip of Elecraft T1 3.5mm jack

int prevBand = -1;

/*------------------------------------------------------------------ */
void setup() {
#ifdef debug
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor (native USB boards)
  Serial.println("FT817 to Elecraft T1 interface");
#endif

  pinMode(TUNE, OUTPUT);
  digitalWrite(TUNE, LOW); 

  pinMode(DATA, INPUT);
}

/*------------------------------------------------------------------ */
void loop() {
      int band;

      band = determineFT817Band();


    if ((band != -1) && (band != prevBand)) {
      #ifdef debug
          Serial.print("   Data: ");
          Serial.println(band);      
      #endif
      setElecraftBand(band);
      prevBand = band;
    }
#ifdef debug
      delay(2000);
#else
      // Low power mode impacts the UART so is not used in debug mode
      int sleepMS = Watchdog.sleep(2000);
#endif     
 }

/*------------------------------------------------------------------ */

int determineFT817Band() {

  // wait 2 milliseconds before the ADC to settle
  delay(2);

  // read the analog in value:
  int sensorValue = analogRead(analogInPin);
  int band = -1;

  for ( int i = 0; i < sizeof(bandTableList) / sizeof(bandTable); i++) {
    if ((sensorValue >= bandTableList[i].adcValue - 25) && (sensorValue <= bandTableList[i].adcValue + 25)) {
       band = bandTableList[i].band;
#ifdef debug
       Serial.println("Band " + bandTableList[i].bandText);
#endif
    };
  };
  return band;
}

/*------------------------------------------------------------------ */
void setElecraftBand(int band) {

  // Pull the TUNE line high for half a second

  digitalWrite(TUNE, HIGH);
  delay(500);
  digitalWrite(TUNE, LOW); 

  // The ATU will pull the DATA line HIGH for 50ms
  while(digitalRead(DATA) == LOW) {
  }
  while(digitalRead(DATA) == HIGH) {
  }
  

  // Wait 10ms
  delay(10);

  // and then send data on the DATA line
  pinMode(DATA, OUTPUT);

  // 1 bits are HIGH for 4ms 
  // 0 bits are HIGH for 1.5ms
  // Gap between digits is 1.5ms LOW

  // 1st bit
  sendBit(band & 8);
  sendBit(band & 4);
  sendBit(band & 2);
  sendBit(band & 1);
  
  // Leave the line LOW
  digitalWrite(DATA, LOW);

  // and switch it back to an input
  pinMode(DATA, INPUT);
}

/*------------------------------------------------------------------ */
void sendBit(int bit) {
  
  digitalWrite(DATA, HIGH);
  if (bit != 0) {
    delay(4);
  } else {
    delayMicroseconds(1500);
  }
  
  digitalWrite(DATA, LOW);
  delayMicroseconds(1500);
}
