#include <Adafruit_ISM330DHCX.h>
#include <Wire.h>

#define ADDRESS     0x6A

#define CTRL1_XL    0x10
#define CTRL3_C     0x12
#define TAP_CFG0    0x56
#define TAP_CFG2    0x58
//#define WAKE_UP_DUR 0x5C
#define FREE_FALL   0x5D
#define MD1_CFG     0x5E

#define ACCELEROMETER_417HZ_2G (6 << 4)
#define INT_CLR_ON_READ        (1 << 7)
#define LATCHED_INT            (1 << 0)
#define INTERRUPTS_EN          (1 << 7)
#define FREE_FALL_THRESHOLD    (3 << 0)
#define FREE_FALL_DURATION     (6 << 3)
#define INT1_FF_INTERRUPT      (1 << 4)  

#define SW_RESET 1

Adafruit_ISM330DHCX ism;

void setup(){
  Serial.begin(115200);
  while(!Serial) delay(10);

  Wire.begin();
  Wire.setClock(400000);
  Wire.setWireTimeout(3000, true);

  i2c_write(ADDRESS, CTRL3_C, SW_RESET);
 
  if(!ism.begin_I2C(ADDRESS)) {
    Serial.println("ISM330DHCX connection error");
  }

  enableFreeFall();
  attachInterrupt(digitalPinToInterrupt(2), freeFallInt, RISING);
}

void loop(){};

void enableFreeFall(){
  i2c_write(ADDRESS, CTRL1_XL, ACCELEROMETER_417HZ_2G);
  i2c_write(ADDRESS, TAP_CFG0, INT_CLR_ON_READ | LATCHED_INT);
  i2c_write(ADDRESS, TAP_CFG2, INTERRUPTS_EN);
  i2c_write(ADDRESS, FREE_FALL, FREE_FALL_DURATION | FREE_FALL_THRESHOLD);
  i2c_write(ADDRESS, MD1_CFG, INT1_FF_INTERRUPT);
}

void freeFallInt(){
  Serial.println("Free falling!");
}

void i2c_write(uint8_t addr, uint8_t reg, uint8_t val){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  uint8_t error = Wire.endTransmission();

  Serial.print(reg, HEX);
  Serial.print(" , ");
  Serial.println(error);
}