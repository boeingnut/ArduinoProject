// Main module   K_bot               angles in Quids, 10 bit ADC  -------------
// 4 - Checking sensor data format    display raw sensors data             

#include <math.h>

#define   GYR_Y                 0                              // Gyro Y (IMU pin #4)
#define   ACC_Z                 1                              // Acc  Z (IMU pin #7)
#define   ACC_X                 2                              // Acc  X (IMU pin #9)

int   STD_LOOP_TIME  =          9;             

int sensorValue[3]  = { 0, 0, 0};
int sensorZero[3]   = { 0, 0, 0}; 
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;
int ACC_angle;
int GYRO_rate;

void setup() {
  analogReference(EXTERNAL);                                   // Aref 3.3V
  Serial.begin(115200);
  delay(100);                                                
  calibrateSensors();
}

void loop() {
// ********************* Sensor aquisition & filtering *******************
  updateSensors();
  ACC_angle = getAccAngle();                                 // in Quids  
  GYRO_rate = getGyroRate();                                 // in Quids per seconds 

// ********************* print Debug info *************************************
  serialOut_sensor();

// *********************** loop timing control **************************
  lastLoopUsefulTime = millis()-loopStartTime;
  if(lastLoopUsefulTime<STD_LOOP_TIME)         delay(STD_LOOP_TIME-lastLoopUsefulTime);
  lastLoopTime = millis() - loopStartTime;
  loopStartTime = millis();
}

void serialOut_sensor() {
static int skip=0;
  if(skip++==20) {                                                        
    skip = 0;
    Serial.print(ACC_angle);          Serial.print(",");
    Serial.print(GYRO_rate);          Serial.print("\n");
  }
}

// Sensors Module  ---------------------------------------------------------------------

void calibrateSensors() {                                       // Set zero sensor values
  long v;
  for(int n=0; n<3; n++) {
    v = 0;
    for(int i=0; i<50; i++)       v += readSensor(n);
    sensorZero[n] = v/50;
  }                                                           
  sensorZero[ACC_Z] -= 200;                        
}

void updateSensors() {                                         // data acquisition
  long v;
  for(int n=0; n<3; n++) {
    v = 0;
    for(int i=0; i<5; i++)       v += readSensor(n);
    sensorValue[n] = v/5 - sensorZero[n];
  }
}

int readSensor(int channel){
  return (analogRead(channel));
}

int getGyroRate() {                                             // ARef=3.3V, Gyro sensitivity=2mV/(deg/sec)
  return int(sensorValue[GYR_Y] * 13.681592);                 // in quid/sec:(1024/360)/1024 * 3.3/0.002)
}

int getAccAngle() {                      
  return arctan2(-sensorValue[ACC_Z], -sensorValue[ACC_X]) + 512;    // in Quid: 1024/(2*PI))
}

int arctan2(int y, int x) {                                    // http://www.dspguru.com/comp.dsp/tricks/alg/fxdatan2.htm
   int coeff_1 = 128;                                          // angle in Quids (1024 Quids=360°)
   int coeff_2 = 3*coeff_1;
   float abs_y = abs(y)+1e-10;
   float r, angle;
   
   if (x >= 0) {
     r = (x - abs_y) / (x + abs_y);
     angle = coeff_1 - coeff_1 * r;
   }  else {
     r = (x + abs_y) / (abs_y - x);
     angle = coeff_2 - coeff_1 * r;
   }
   if (y < 0)      return int(-angle); 
   else            return int(angle);
}
