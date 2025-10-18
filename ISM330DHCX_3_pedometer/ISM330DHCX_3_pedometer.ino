#include <Adafruit_ISM330DHCX.h>
#include <Wire.h>

/*
  This is a register-level i2c implementation because library doesn't 
  handle pedometer for ISM330.

  FUNC_CFG_ACCESS enables and disables embedded functions registers
  where pedometer is located. It switches the register addresses 
  so if you want to access default registers you need to disable it
  again. It's tricky to do if you don't know how to do bare-metal 
  programming.

  If you upload a different sketch that won't use this feature you must
  reset the sensor (not the Arduino itself), otherwise it won't work.
*/

#define ADDRESS           0x6A
#define FUNC_CFG_ACCESS   0x01
#define EMB_FUNC_EN_A     0x04
#define MD1_CFG           0x5E
#define EMB_FUNC_INT1     0x0A
#define EMB_FUNC_INIT_A   0x66

#define FUNC_CFG_ACCESS_EN  (1 << 7)
#define PEDO_EN             (1 << 3)
#define INT1_EMB_FUNC       (1 << 1)
#define INT1_STEP_DETECTOR  (1 << 3)
#define STEP_DET_INIT       (1 << 3)

#define EMB_FUNC_STEP_COUNTER_L 0x62

Adafruit_ISM330DHCX ism;

void setup() {
  Serial.begin(115200);
  while(!Serial) delay(10);

  Wire.begin();

  if(!ism.begin_I2C(0x6A)) {
    Serial.println("ISM330DHCX connection error");
  }

  enablePedometer();
}

void loop() {
  uint16_t steps = readPedometer();

  //the pedometer will append new steps after 10 steps in a row.
  Serial.print("Number of steps: ");
  Serial.println(steps);

  delay(100);
}


void enablePedometer(){
  //interrupt routing enable
  //i2c_write(ADDRESS, MD1_CFG, INT1_EMB_FUNC); 
  
  i2c_write(ADDRESS, FUNC_CFG_ACCESS, FUNC_CFG_ACCESS_EN);
  i2c_write(ADDRESS, EMB_FUNC_EN_A, PEDO_EN);
   
  //routing enable step detector
  //i2c_write(ADDRESS, EMB_FUNC_INT1, INT1_STEP_DETECTOR); 

  i2c_write(ADDRESS, EMB_FUNC_INIT_A, STEP_DET_INIT);

  //switch back to normal registers from embedded functions
  //otherwise you can't read basic registers
  //i2c_write(ADDRESS, FUNC_CFG_ACCESS, 0); 
}

uint16_t readPedometer(){
  Wire.beginTransmission(ADDRESS);
  Wire.write(EMB_FUNC_STEP_COUNTER_L);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDRESS, 2, true); 

  uint16_t steps = (Wire.read() | Wire.read() << 8);

  return steps;
}

void i2c_write(uint8_t addr, uint8_t reg, uint8_t val){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}