#include <Adafruit_ISM330DHCX.h>
#include <Wire.h>

#define ADDRESS     0x6A

#define CTRL1_XL    0x10
#define CTRL3_C     0x12

#define TAP_CFG0    0x56
#define TAP_CFG1    0x57
#define TAP_CFG2    0x58
#define INT1_CTRL   0x0D
#define TAP_THS_6D  0x59
#define INT_DUR2    0x5A
#define MD1_CFG     0x5E
#define TAP_SRC     0x1C

#define INT_CLR_ON_READ (1 << 7)
#define TAP_X_EN        (1 << 3)
#define TAP_Y_EN        (1 << 2)
#define TAP_Z_EN        (1 << 1)
#define LATCHED_INT     (1 << 0)

#define TAP_THRESHOLD_X 9
#define TAP_THRESHOLD_Y 9
#define TAP_THRESHOLD_Z 9
#define TAP_SHOCK       2
#define TAP_QUIET       (1 << 2)

#define INTERRUPTS_EN   (1 << 7)
#define INT1_SINGLE_TAP (1 << 6)
#define SINGLE_TAP      (1 << 6)

#define ACCEL_ODR       (6 << 4)
#define SW_RESET        1

#define isTapX          (1 << 2)
#define isTapY          (1 << 1)
#define isTapZ          (1 << 0)
#define tapSign         (1 << 3)

Adafruit_ISM330DHCX ism;

volatile int val = 0;
volatile int counter = 0;
volatile bool isTappedFlag = false;

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

  enableSingleTap();
  attachInterrupt(digitalPinToInterrupt(2), readTap, RISING);
}

void loop(){

  if(isTappedFlag){
    Wire.beginTransmission(ADDRESS);
    Wire.write(TAP_SRC);
    Wire.endTransmission(false);
    Wire.requestFrom(ADDRESS, 1, false); 

    volatile uint8_t tap = Wire.read();

    if(tap & isTapX){
      if(tap & tapSign) Serial.print("+X tap ");
      else Serial.print("-X tap ");
    }
    else if(tap & isTapY){
      if(tap & tapSign) Serial.print("+Y tap ");
      else Serial.print("-Y tap ");
    }
    else if(tap & isTapZ){
      if(tap & tapSign) Serial.print("+Z tap ");
      else Serial.print("-Z tap ");
    }

    Serial.println(counter);
    isTappedFlag = false;
  }

}

void enableSingleTap(){
  i2c_write(ADDRESS, CTRL1_XL, ACCEL_ODR);
  i2c_write(ADDRESS, TAP_CFG0, INT_CLR_ON_READ  | TAP_X_EN | TAP_Y_EN | TAP_Z_EN | LATCHED_INT);
  i2c_write(ADDRESS, TAP_CFG1, TAP_THRESHOLD_X);
  i2c_write(ADDRESS, TAP_CFG2, INTERRUPTS_EN | TAP_THRESHOLD_Y);
  i2c_write(ADDRESS, TAP_THS_6D, TAP_THRESHOLD_Z);
  i2c_write(ADDRESS, INT_DUR2, TAP_SHOCK | TAP_QUIET);
  i2c_write(ADDRESS, MD1_CFG, INT1_SINGLE_TAP);
}

void readTap(){
  counter++;
  isTappedFlag = true;
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