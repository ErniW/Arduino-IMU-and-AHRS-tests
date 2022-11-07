#include <Adafruit_ISM330DHCX.h>
#include <Wire.h>

#define ADDRESS     0x6A

#define CTRL1_XL    0x10
#define CTRL3_C     0x12
#define TAP_CFG0    0x56
#define TAP_CFG2    0x58
#define TAP_THS_6D  0x59
#define CTRL8_XL    0x17
#define MD1_CFG     0x5E
#define DRD_SRC     0x1D

#define ACCELEROMETER_417HZ_2G (6 << 4)
#define INT_CLR_ON_READ        (1 << 7)
#define LATCHED_INT            (1 << 0)
#define INTERRUPTS_EN          (1 << 7)
#define TILT_THRESHOLD_60DEG   (2 << 5)
#define LOW_PASS_ON_6D         (1 << 0)
#define INT1_6D                (1 << 2)

#define TILT_ZH (1 << 5)
#define TILT_ZL (1 << 4)
#define TILT_YH (1 << 3)
#define TILT_YL (1 << 2)
#define TILT_XH (1 << 1)
#define TILT_XL (1 << 0)

#define SW_RESET 1

volatile bool isTiltFlag = false;

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

  enableTilt6D();
  attachInterrupt(digitalPinToInterrupt(2), readTilt, RISING);
}

void loop(){
  if(isTiltFlag){

    Wire.beginTransmission(ADDRESS);
    Wire.write(DRD_SRC);
    Wire.endTransmission(false);
    Wire.requestFrom(ADDRESS, 1, false); 

    volatile uint8_t tilt = Wire.read();

    Serial.print("Orientation: ");

    if(tilt & TILT_ZH) Serial.println("ZH");
    else if(tilt & TILT_ZL) Serial.println("ZL");
    else if(tilt & TILT_YH) Serial.println("YH");
    else if(tilt & TILT_YL) Serial.println("YL");
    else if(tilt & TILT_XH) Serial.println("XH");
    else if(tilt & TILT_XL) Serial.println("XL");

    isTiltFlag = false;
  }
}

void enableTilt6D(){
  i2c_write(ADDRESS, CTRL1_XL, ACCELEROMETER_417HZ_2G);
  i2c_write(ADDRESS, TAP_CFG0, INT_CLR_ON_READ | LATCHED_INT);
  i2c_write(ADDRESS, TAP_CFG2, INTERRUPTS_EN);
  i2c_write(ADDRESS, TAP_THS_6D, TILT_THRESHOLD_60DEG);
  i2c_write(ADDRESS, CTRL8_XL, LOW_PASS_ON_6D );
  i2c_write(ADDRESS, MD1_CFG, INT1_6D);
}

void readTilt(){
  isTiltFlag = true;
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