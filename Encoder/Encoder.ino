#include <Encoder.h>

Encoder enc(2,3);

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Test:");
}

long encposition  = -999;
int pos;

void loop() {
  long encpos;
  encpos = enc.read();
  pos = encpos/4;
  Serial.print("Encoder = ");
  Serial.print(pos);
  Serial.println();
  encposition = pos;
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    enc.write(0);
  }
  delay(1000);
}

