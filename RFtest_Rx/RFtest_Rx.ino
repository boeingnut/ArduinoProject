#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

float temp1, hum1;

RF24 radio(48, 49);
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };

void setup(void) {
Serial.begin(9600);
radio.begin();
radio.setDataRate(RF24_250KBPS);
radio.openReadingPipe(1, pipes[1]);
radio.openReadingPipe(2, pipes[2]);
radio.startListening();
}

void loop(void)
{
if ( radio.available() )
{
  radio.read(&temp1, sizeof(temp1));
  Serial.println(temp1);
}
}
