/*
  SD card test

 This example shows how use the utility libraries on which the'
 SD library is based in order to get info about your SD card.
 Very useful for testing a card when you're not sure whether its working or not.

 The circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
        Pin 4 used here for consistency with other Arduino examples


 created  28 Mar 2011
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe
 */
// include the SD library:
#include <SPI.h>
#include <SD.h>

#include <util/delay_basic.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int sSpeakerPin = 15;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // High impedance
  pinMode(sSpeakerPin, INPUT);

  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

}

void loop(void) {  
  // TODO: Stream properly, this will skip
  int8_t block_mem[512];
  for(unsigned long block=0;;++block) {
    if(!card.readBlock(block, (uint8_t*)(&block_mem[0]))) {
      Serial.println("Failed to read block");
    }
    
    for(unsigned int idx=0;idx<512;++idx) {
      const int8_t sample = block_mem[idx];
      
      // 4 clock cyles per iteration makes a maximum of 255*4 = 1020 cycles.
      // 16mhz / 16khz = 1000 cycles per sample.
      // Close enough!
      
      // Dead time      
      DDRD = DDRD & B00000011;
      _delay_loop_2(1);
      
      // TODO: Direct pins for timing
      const uint8_t sample_mag = abs(sample);
      if(sample_mag < 125) {
        _delay_loop_2(255 - (sample_mag << 1));
      }
      
      if(sample != 0) {
        PORTD = (sample > 0) ? 0xFF : 0x00;
        DDRD = DDRD | B11111100;
        _delay_loop_2(sample_mag << 1);
      }
    }
    DDRD = DDRD & B00000011;
  }
}
