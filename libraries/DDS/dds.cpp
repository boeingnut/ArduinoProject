/*  dds.cpp version 2012082802

    DDS Interface Library

    Anthony Good - anthony dot good at gmail dot com
    K3NG

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


Sample code:
 
#include <stdio.h>
#include <dds.h>

dds ddschip(DDS9850, 8, 6, 7, 100000000LL);

void setup()

{ 
  ddschip.calibrate(0.000050); 
}

void loop()
{
  ddschip.setfrequency(7000000LL,POWER_UP);
  delay(2000);
  ddschip.setfrequency(8000000LL,POWER_UP);
  delay(2000);  
}


*/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <dds.h>

#define DEBUG_DDS


static unsigned long clock_mhz;
static float calibration = 0;
static uint8_t data_pin;
static uint8_t load_pin;
static uint8_t clock_pin;
static uint8_t chip_type;
static uint8_t clock_multiplier = 0;
static uint8_t triangle_wave = 0;


unsigned long dds_bits(unsigned long frequency)
{
  unsigned long calculated_bits = 0;
	
	uint8_t dds_number_of_bits = 32;
  
  // 32 bit word for 9850/1 DDS
  // word = ( f * (2^32) ) / dds_clock_speed

	if (chip_type == DDS9834){
	  dds_number_of_bits = 28;
	}

  calculated_bits = (unsigned long) (( (float) frequency * pow(2,dds_number_of_bits) ) / (float) (clock_mhz - (clock_mhz*calibration)));


  #ifdef DEBUG_DDS
  Serial.write("dds_bits: freq: ");
  Serial.print(frequency);
  Serial.write(" calibration: ");
  Serial.print(calibration);
  Serial.write(" clock_mhz: ");
  Serial.print(clock_mhz);
  Serial.write(" calculated_bits: ");
  Serial.println(calculated_bits);
  #endif
 
  
  return calculated_bits;
}



void send_dds (uint8_t bit_to_send)
{
  
  // send a bit to the DDS chip
  // we set the bit on the data pin then take the clock pin high to get the chip to "take" it
  
  if (bit_to_send) {
    digitalWrite(data_pin,HIGH);
    digitalWrite(clock_pin,HIGH);
    digitalWrite(clock_pin,LOW);
    digitalWrite(clock_pin,HIGH);
		if (chip_type != DDS9834) {
			digitalWrite(data_pin,LOW); // the 9834 reads the data pin on falling edge of clock pin
		}
    #ifdef DEBUG_DDS
    Serial.write("1");
    #endif
  } else {
    digitalWrite(data_pin,LOW);
    digitalWrite(clock_pin,HIGH);
    digitalWrite(clock_pin,LOW);
    digitalWrite(clock_pin,HIGH);
    #ifdef DEBUG_DDS
    Serial.write("0");
    #endif
  }
  
}


dds::dds(uint8_t dds_chip, uint8_t dds_data_pin, uint8_t dds_load_pin, uint8_t dds_clock_pin, unsigned long dds_clock_mhz) {


  clock_mhz = dds_clock_mhz;
  data_pin = dds_data_pin;
  load_pin = dds_load_pin;
  clock_pin = dds_clock_pin;
  chip_type = dds_chip;


  pinMode(load_pin,OUTPUT);
  if (chip_type == DDS9834) {
    digitalWrite(load_pin,HIGH); // documentation for 9834 seems to indicate the load pin should normally be kept high
  } else {
    digitalWrite(load_pin,LOW);
  }
  pinMode(clock_pin,OUTPUT);
  digitalWrite(clock_pin,LOW);
  pinMode(data_pin,OUTPUT);
  digitalWrite(data_pin,LOW);

}

//void dds::calibrate(unsigned long dds_calibration) {
void dds::calibrate(float dds_calibration) {

  calibration = dds_calibration;

}

void dds::set_clock_multiplier(uint8_t state) {

  if (state) {
    clock_multiplier = 1;
  } else {
    clock_multiplier = 0;
  }

}

void dds::set_triangle_wave(uint8_t state) {

  if (state) {
    triangle_wave = 1;
  } else {
    triangle_wave = 0;
  }

}

//---------------------------------------------------

void dds::setfrequency(unsigned long frequency)
{

  unsigned long dds_32_bit_word = dds_bits(frequency);

  #ifdef DEBUG_DDS
  Serial.write("dds::setfrequency: freq: ");
  Serial.print(frequency);
  Serial.write(" clock_mhz: ");
  Serial.print(clock_mhz);
  Serial.write(" calib: ");
  Serial.print(calibration);
  Serial.write(" data_pin: ");
  Serial.print(data_pin);
  Serial.write(" clock_pin: ");
  Serial.print(clock_pin);
  Serial.write(" load_pin: ");
  Serial.print(load_pin);
  Serial.write(" dds_32_bit_word: ");
  Serial.println(dds_32_bit_word);
  #endif
 
  if (chip_type == DDS9834) {
	
		digitalWrite(load_pin,LOW);
		
	  for (int x = 0; x < 16; x++) {  // send control word
		  if ((x == 2) || (x == 7)) {
			  send_dds(1);  //DB13 = 1 - This allows a complete word to be loaded into a frequency
                      //register in two consecutive writes. The first write contains 14 LSBs. 
											//The second write contains 14 MSBs
											//
											//RESET bit DB8 is set to 1. This resets internal registers to 0, which
                      //corresponds to an analog output of midscale.
			} else {
			  if ((x == 14) and (triangle_wave)) {  // set MODE bit if triangle wave is activated
				  send_dds(1);
			  } else {
		      send_dds(0);
				}
			}
		}
		
		send_dds(0); // (DB15, DB14 = 01)
		send_dds(1); 
		
		// send 14 LSBs
		for (int x = 13; x > -1; x--) {
			if (bitRead(dds_32_bit_word,x)) {
				send_dds(1);
			} else {
				send_dds(0);
			}  		
		}
 
 		send_dds(0); // (DB15, DB14 = 01)
		send_dds(1); 
		
		// send 14 MSBs
		for (int x = 27; x > 13; x--) {
			if (bitRead(dds_32_bit_word,x)) {
				send_dds(1);
			} else {
				send_dds(0);
			}  		
		} 
		
		
		dds_32_bit_word = 0xC0002000;
		
		for (int x = 31; x > -1; x--) {
			if (bitRead(dds_32_bit_word,x)) {
				send_dds(1);
			} else {
				send_dds(0);
			}  		
		} 
		
		
		
		
		digitalWrite(load_pin,HIGH);
 
  } else {  // 9850 and 9851
	
    digitalWrite(load_pin,LOW);
 
    // send the 32 bit word, least significant bit first
		for (int x = 0; x < 32; x++) {
		//if ((x % 4) == 0) { Serial.write("|"); }
			if (bitRead(dds_32_bit_word,x)) {
				send_dds(1);
			} else {
				send_dds(0);
			}  
		}

		// send the control byte at the end
		for (int y = 0; y < 8; y++) {
			if ((y == 0) && (chip_type == DDS9851) && (clock_multiplier)) {
				send_dds(1);  // set the 6 X multiplier control bit on a 9851 if it's activated
			} else {
				send_dds(0);
			}

		}

		// take the load pin high momentarily to tell the DDS chip to run with the new setting 
		digitalWrite(load_pin,HIGH);
		//delay(1);
		digitalWrite(load_pin,LOW);

		

	
	}
	
	#ifdef DEBUG_DDS
	Serial.write("\n\rdds::setfrequency: loaded\n\r");
	#endif
	  
	  
}

