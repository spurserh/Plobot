
#include <SPI.h>
#include <SD.h>
#include "SDAudio.h"

SDAudio card;

const int chipSelect = 4;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  SDAudio::Setup();

  Serial.print("\nInitializing SD card...");
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    return;
  }
}

void loop(void) {
  card.StreamBlocks(700, 100);
  delay(750);
}
