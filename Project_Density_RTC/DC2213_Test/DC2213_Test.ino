#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "SPI.h"
#include "Wire.h"
#include "stdio.h"
#include "math.h"

#include "configuration_constants.h"
#include "table_coeffs.h"

#define SCK 13
#define MISO 12
#define MOSI 11
#define CS 2

// Function prototypes

// -----------------------------------------------------------------
// 		          Configure LTC2983
// -----------------------------------------------------------------


void setup() {
	Serial.begin(9600);
        pinMode(SCK, OUTPUT);             //! 1) Setup SCK as output
        pinMode(MOSI, OUTPUT);            //! 2) Setup MOSI as output
        pinMode(CS, OUTPUT);              //! 3) Setup CS as output
        pinMode(MISO, INPUT);             //! 4) Setup MISO as input
        SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	digitalWrite(CS, HIGH);
        delay(500); 
        configure_channels();
	configure_global_parameters();
} 


void configure_channels() {
  byte channel_number;
  long channel_assignment_data;   

  // ----- Channel 2: Assign Direct ADC -----
  channel_assignment_data =
    (long) SENSOR_TYPE__DIRECT_ADC |
    (long) DIRECT_ADC_DIFFERENTIAL;
  assign_channel(2, channel_assignment_data);

}




void assign_channel(int channel_num, long channel_data) 
{
	int bytes_per_coeff = 4;
        long start_address = 0x200+4*(channel_num-1);
	initialize_memory_write(start_address);
	write_coefficient(channel_data, bytes_per_coeff);
	finish_memory_write();
}

void configure_global_parameters() {
	// -- Set rejection frequency and temperature unit
	write_single_byte(0xF0, TEMP_UNIT__C | REJECTION__50_60_HZ);
	// -- Set any extra delay between conversions (in this case, 0*100us)
	write_single_byte(0xFF, 0);
}

// -----------------------------------------------------------------

// -----------------------------------------------------------------
//                  Memory write functions
// -----------------------------------------------------------------
void initialize_memory_write(long start_address) {
	digitalWrite(CS, LOW);
  delay(10);
	spi_write(0x02); // instruction Byte Write
	spi_write(highByte(start_address)); // Address MSB Byte
	spi_write(lowByte(start_address)); // Address LSB Byte
}

void finish_memory_write() {
	digitalWrite(CS, HIGH);
}

void write_coefficient(long coeff, int bytes_per_coeff) {
	int data_byte;
	for (int i = bytes_per_coeff - 1; i >= 0; i--) {
		  data_byte = coeff >> (i*8);
			spi_write(data_byte);
	}
}

void write_single_byte(long start_address, int single_byte) {
	initialize_memory_write(start_address);
	spi_write(single_byte);
	finish_memory_write();
}

void spi_write(int8_t  data)  // Byte to be written to SPI port
{
  SPDR = data;                  //! 1) Start the SPI transfer
  while (!(SPSR & _BV(SPIF)));  //! 2) Wait until transfer complete
}

// -----------------------------------------------------------------
//                  Memory read functions
// -----------------------------------------------------------------
void initialize_memory_read(long start_address) {
  digitalWrite(CS, LOW);
  delay(10);
  spi_write(0x03); // instruction Byte read
  spi_write(highByte(start_address)); // Address MSB Byte
  spi_write(lowByte(start_address)); // Address LSB Byte
}

void finish_memory_read() {
  digitalWrite(CS, HIGH);
}

int8_t spi_read(int8_t  data) //!The data byte to be written
{
  SPDR = data;                  //! 1) Start the SPI transfer
  while (!(SPSR & _BV(SPIF)));  //! 2) Wait until transfer complete
  return SPDR;                  //! 3) Return the data read
}

// -----------------------------------------------------------------
// 		          Channel conversion
// -----------------------------------------------------------------
void convert_channel(byte channel_number)
{
  // initiate a new conversion
  
  initialize_memory_write(0);
  spi_write(0b10000000 | channel_number); // start a conversion
  finish_memory_write();
  while (conversion_done()==0);  // wait for conversion to complete
}

boolean conversion_done()
{
  initialize_memory_read(0);
  byte data=spi_read(0x00);
  finish_memory_read();
  delay(10);
  return(data & 0b01000000);
}



// -----------------------------------------------------------------
// 			Getting temperature results
// -----------------------------------------------------------------

float read_temperature_results(int channel_number) {
	unsigned char raw_results[4];
	get_raw_results(READ_CH_BASE, channel_number, raw_results);
	float signed_float = convert_to_signed_float(raw_results);
	float temperature_result = get_temperature(signed_float);
	print_temperature_result(channel_number, temperature_result);
	print_fault_data(raw_results[3]);
	return (temperature_result);
}

float read_direct_adc_results(int channel_number) {
	unsigned char raw_results[4];
	get_raw_results(READ_CH_BASE, channel_number, raw_results);
	float signed_float = convert_to_signed_float(raw_results);	
	float direct_adc_reading = get_direct_adc_reading(signed_float);
	print_direct_adc_reading(channel_number, direct_adc_reading);
	print_fault_data(raw_results[3]);
	return (direct_adc_reading);
}

void get_raw_results(long base_address, int channel_number, unsigned char results[4]) {
  long address = base_address+4*(channel_number-1);
  initialize_memory_read(address);
  
  results[3]=spi_read(0x00); // fault data
  results[2]=spi_read(0x00); // MSB result byte
  results[1]=spi_read(0x00); // 2nd result byte
  results[0]=spi_read(0x00); // LSB result byte
  finish_memory_read();
}

float convert_to_signed_float(unsigned char results[4]) {
  // Get the last 24 bits of the results (the first 8 bits are status bits)
  long x = 0L;
  x= x | ((unsigned long) results[2]<<16)
       | ((unsigned long) results[1]<<8)
       | ((unsigned long) results[0]);
	
  // Convert a 24-bit two's complement number into a 32-bit two's complement number
  boolean sign;  
  if ((results[2]&0b10000000)==128) sign=true; else sign=false;	
  if (sign) x=x | 0xFF000000;

  return float(x);  
}

float get_temperature(float x) {
  // The temperature format is (14, 10) so we divide by 2^10
  return x/1024;
}

float get_direct_adc_reading(float x) {
	// The direct ADC format is (3, 21) so we divide by 2^21
	return x/2097152;
}

void print_temperature_result(int channel_number, float temperature_result) {
  Serial.print("Channel=");
  Serial.print(channel_number);
  Serial.print(" Temperature Result =");
  Serial.print(temperature_result);
}

void print_direct_adc_reading(int channel_number, float direct_adc_reading) {
  Serial.print("Channel=");
  Serial.print(channel_number);
  Serial.print(" Direct ADC reading in V=");
  Serial.print(direct_adc_reading,4);
}

// Translate the fault byte into usable fault data and print it out
void print_fault_data(unsigned char fault_byte) {
  Serial.print("  FAULT DATA=");
  Serial.println(fault_byte,BIN);

  if ((fault_byte&SENSOR_HARD_FAILURE)>0) Serial.println("*SENSOR HARD FALURE*");
  if ((fault_byte&ADC_HARD_FAILURE)>0) Serial.println("*ADC_HARD_FAILURE*");
  if ((fault_byte&CJ_HARD_FAILURE)>0) Serial.println("*CJ_HARD_FAILURE*");
  if ((fault_byte&CJ_SOFT_FAILURE)>0) Serial.println("*CJ_SOFT_FAILURE*");
  if ((fault_byte&SENSOR_ABOVE)>0) Serial.println("*SENSOR_ABOVE*");
  if ((fault_byte&SENSOR_BELOW)>0) Serial.println("*SENSOR_BELOW*");
  if ((fault_byte&ADC_RANGE_ERROR)>0) Serial.println("*ADC_RANGE_ERROR*");
  if ((fault_byte&VALID)!=1) Serial.println("!!!!!!! INVALID READING !!!!!!!!!");
  if (fault_byte==0b11111111) Serial.println("&&&&&&&&&& CONFIGURATION ERROR &&&&&&&&&&&&");
  Serial.println(" "); 
}

// -----------------------------------------------------------------
// 		        Getting raw results -
//    voltage (for thermocouples), resistance (for RTDs/thermistors)
// -----------------------------------------------------------------

float read_voltage_or_resistance_results(int channel_number) {
	unsigned char raw_results[4];
	get_raw_results(VOUT_CH_BASE, channel_number, raw_results);
	float signed_float = convert_vr_to_signed_float(raw_results);
	float voltage_or_resistance_result = get_voltage_or_resistance(signed_float);
	print_voltage_or_resistance_result(channel_number, voltage_or_resistance_result);
	Serial.println(voltage_or_resistance_result,3);
	return (voltage_or_resistance_result);
}

float convert_vr_to_signed_float(unsigned char results[4]) {
  long x = 0L;
  x= x | ((unsigned long) results[3]<<24)
       | ((unsigned long) results[2]<<16)
       | ((unsigned long) results[1]<<8)
       | ((unsigned long) results[0]);
  return float(x);
}

float get_voltage_or_resistance(float x) {
	// The format is (14, 10) so we divide by 2^10
	return x/1024;
}

void print_voltage_or_resistance_result(int channel_number, float voltage_or_resistance_result) {
  Serial.print("Channel=");
  Serial.print(channel_number);
  Serial.print(" Voltage or resistance=");
  Serial.print(voltage_or_resistance_result,3);
  Serial.print("\n");
}

// -------------- Run the LTC2983 -------------------------------------
void loop() { 
  float temperature_result;
  byte channel_number; 
  
  int channels_to_measure[] = {2};
  int num_measured_channels = sizeof(channels_to_measure)/sizeof(channels_to_measure[0]);

  
  for (int i = 0; i < num_measured_channels; i++) {
    channel_number = channels_to_measure[i];
    convert_channel(channel_number);

    //read_voltage_or_resistance_results(channel_number);
    read_direct_adc_results(channel_number);
    //read_temperature_results(channel_number);

  }
  delay(1000);
}
