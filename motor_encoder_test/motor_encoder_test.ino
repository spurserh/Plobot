
#include <DCMotor.h>

const int sEncoderPins[] = {0, 1};
const int sEncoderCount = sizeof(sEncoderPins) / sizeof(*sEncoderPins);

int M0EN = 12;
int M01  = 19;
int M02 =  20;

int M1EN =  13;
int M11 =  21;
int M12 =  22;

DCMotor motor0(M0EN, M01, M02);
DCMotor motor1(M1EN, M11, M12);

void setup() {
  for(int i=0;i<sEncoderCount;++i) {
    pinMode(sEncoderPins[i], INPUT);
    digitalWrite(sEncoderPins[i], HIGH);
  }
  Serial.begin(9600);
}

void count_encoders_for(int ms)
{
  int last_states[sEncoderCount];
  int counts[sEncoderCount];
  for(int i=0;i<sEncoderCount;++i) {
    last_states[i] = digitalRead(sEncoderPins[i]);
    counts[i] = 0;
  }
  for(unsigned long start_m=millis();(millis() - start_m) < ms;) {
    for(int i=0;i<sEncoderCount;++i) {
      int state = digitalRead(sEncoderPins[i]);
      if(state != last_states[i]) {
        ++counts[i];
        last_states[i] = state;
      }
    }
  }
  Serial.print("Counts:");
  for(int i=0;i<sEncoderCount;++i) {
    Serial.print(counts[i]);
    Serial.println(" ");
  }
  Serial.println();
}

void loop() {
  for(int m0=0;m0<2;++m0) {
    for(int m1=0;m1<2;++m1) {
      Serial.print("Beginning test: ");
      Serial.print(m0);
      Serial.print(" ");
      Serial.print(m1);
      Serial.println();
      if(m0)
        motor0.setSpeed(80);
      else
        motor0.brake();
      if(m1)
        motor1.setSpeed(80);
      else 
        motor1.brake();
      count_encoders_for(2000);
      motor0.brake();
      motor1.brake();
      delay(1000);
    }
  }
}
