/*----------------------------------------------
 * This code collects a temperature measurement
 * from up to 4 Adafruit ADT7410 breakout boards
 * every second over I2C and writes them to serial
 * output in the following format
 * "(Addr, TempC, tempF): ..." 
 * where ... is the address and data formatted in
 * the schema shown in parenthesis. Example output
 * for 2 sensors:
 * "(Addr, TempC, TempF): 0x48, 24.2500,75.6500; 0x49, 23.9375,75.0875"
 * ---------------------------------------------*/

#include <Wire.h>
#include "Adafruit_ADT7410.h"

// See #6 at https://forum.arduino.cc/index.php?topic=141908.0

// Change this to the number of sensors in your configuration (max 4)
#define NUM_TEMP_SENSORS 4


//**********************************
// DON'T CHANGE CODE BELOW THIS LINE
//**********************************

// ADT7410 temperature sensor array and their addresses
const int max_temp_sensors = 4;
Adafruit_ADT7410* temp_sensors[max_temp_sensors]; // array of 4 pointers to Adafruit_ADT7410s
uint8_t sensor_addresses[] = {0x48, 0x49, 0x4A, 0x4B};

// Create ADT7410 temperature sensor objects
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();
Adafruit_ADT7410 tempsensors = Adafruit_ADT7410();

void setup() {
  Serial.begin(115200);
  delay(250);

  for (int i=0; i<NUM_TEMP_SENSORS; i++) {
    temp_sensors[i] = new Adafruit_ADT7410();
    // Make sure the sensor is found
    if (temp_sensors[i]->begin(sensor_addresses[i])) Serial.print("Found ADT7410 ");
    else Serial.print("Couldn't find ADT7410 ");
    Serial.print(i);
    Serial.print(" at address ");
    print_serial_hex(sensor_addresses[i]);
    Serial.println("");
   };

  // sensors take 250 ms to get first readings
  delay(250);
}

void loop() {

  float temp_c[max_temp_sensors];
  float temp_f[max_temp_sensors];
  
  for (int i=0; i<NUM_TEMP_SENSORS; i++) {
    temp_c[i] = temp_sensors[i]->readTempC();
    temp_f[i] = temp_c[i] * 9.0 / 5.0 + 32;
  };

  Serial.print("(Addr, TempC, TempF): ");
  for (int i=0; i<NUM_TEMP_SENSORS; i++) {
    print_serial_hex(sensor_addresses[i]);
    Serial.print(",");
    Serial.print(temp_c[i], 4); Serial.print(",");
    Serial.print(temp_f[i], 4); //Serial.print("*F");
    if (i == NUM_TEMP_SENSORS-1) Serial.println("");
    else Serial.print(";");
  };
  //Serial.println("");


//  // Read and print out the temperature, then convert to *F
//  float c = tempsensor.readTempC();
//  float f = c * 9.0 / 5.0 + 32;
//
//  //write_config_register(0x4B);
//  Serial.print("Temp: "); Serial.print(c, 4); Serial.print("*C\t"); 
//  Serial.print(f, 4); Serial.println("*F");

  delay(1000);
}


/////////////////////////////////////////////////////////

void print_serial_hex(uint8_t n) {
  Serial.print("0x");
  Serial.print(n < 16 ? "0" : "");
  Serial.print(n, HEX);
  //Serial.print(" ");
}

// See Answer #6 at https://forum.arduino.cc/index.php?topic=566180.0 for Wire writes and reads

uint8_t read_config_register() { 
  Wire.beginTransmission(0x48);
  Wire.write(0x03); //config register address
  Wire.endTransmission();

  Wire.requestFrom(0x48, 1);
  uint8_t buf = Wire.read();
  Serial.print("read_config_register Config register: ");
  print_serial_hex(buf);
  Serial.print("\t");
  return buf;
}

// THE FUNCTIONS BELOW DON'T WORK YET

bool write_config_register(uint8_t new_config) { 
  Wire.beginTransmission(0x48);
  Wire.write(0x03); //config register address
  Wire.write(new_config); //config register address
  return (Wire.endTransmission()==0);
  //return (Wire.endTransmission(true)==0);
}

bool set_16bit_temperature(uint8_t i2c_address) {
  uint8_t config_byte = read_config_register();
  uint8_t new_config = config_byte | 0x40;
  bool result = write_config_register(new_config);
  Serial.println("");
  Serial.print("set_16bit_temperature result: ");
  Serial.println(result);
  delayMicroseconds(500);
  return result;
}
