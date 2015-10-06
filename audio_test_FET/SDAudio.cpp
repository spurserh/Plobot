
#include <SPI.h>
#include <SD.h>

#include "SDAudio.h"

const int sSpeakerPin = 15;

void SDAudio::Setup()
{
  pinMode(sSpeakerPin, OUTPUT);
  // Off: Coupling cap
  OCR2A = 0;
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  
  // 62.5Khz fast PWM

  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));

  // Set fast PWM mode  (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);

  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
}

boolean SDAudio::StreamBlocks(unsigned long block, unsigned long count)
{
  boolean ret = true;
    
  // Charge coupling cap softly
  for(int i=0;i<128;++i) {
    OCR2A = i;
    delayMicroseconds(1000);
  }
    
  const unsigned long to = block + count;
  uint8_t block_mem[512];
  for(;block < to;++block) {
    if(!Sd2Card::readBlock(block, (uint8_t*)(&block_mem[0]))) {
      ret = false;
      goto done;
    }
    
    for(unsigned int idx=0;idx<512;++idx) {
      OCR2A = block_mem[idx];
      
      // 16khz
      delayMicroseconds(62);
    }
  }
  
    // Discharge coupling cap softly
  for(int i=128;i>=0;--i) {
    OCR2A = i;
    delayMicroseconds(1000);
  }
  
  
 done:
  return ret;
}
