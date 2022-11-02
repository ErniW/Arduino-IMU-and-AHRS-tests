#include <Adafruit_ISM330DHCX.h>
#include <Wire.h>

#define ADDRESS 0x6A
#define FUNC_CFG_ACCESS 0x01
#define EMB_FUNC_EN_A 0x04
#define MD1_CFG 0x5E
#define EMB_FUNC_INT1 0x0A
#define EMB_FUNC_INIT_A 0x66

#define FUNC_CFG_ACCESS_EN (1 << 7)
#define PEDO_EN (1 << 3)
#define INT1_EMB_FUNC (1 << 1)
#define INT1_STEP_DETECTOR (1 << 3)
#define STEP_DET_INIT (1 << 3)

#define EMB_FUNC_STEP_COUNTER_L 0x62

Adafruit_ISM330DHCX ism;

void setup() {
  Serial.begin(115200);
  while(!Serial) delay(10);

  Wire.begin();

  if(!ism.begin_I2C(0x6A)) {
    Serial.println("ISM330DHCX connection error");
  }

  Wire.beginTransmission(ADDRESS);
  Wire.write(0x0F);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDRESS, 1, true); 
  Serial.print("ADDRESS: ");
  Serial.println(Wire.read());

  enablePedometer();
}

void loop() {
  uint16_t steps = readPedometer();

  Serial.print("Number of steps: ");
  Serial.println(steps);

  delay(100);
}


void enablePedometer(){
  
  Serial.println("enabling...");
  i2c_write(ADDRESS, MD1_CFG, INT1_EMB_FUNC); //interrupt routing en embedded functions 
  
  i2c_write(ADDRESS, FUNC_CFG_ACCESS, FUNC_CFG_ACCESS_EN); //func cfg access
  i2c_write(ADDRESS, EMB_FUNC_EN_A, PEDO_EN); // enable pedometer embedded functions
   
  i2c_write(ADDRESS, EMB_FUNC_INT1, INT1_STEP_DETECTOR); //routing enable step detector embedded function
  i2c_write(ADDRESS, EMB_FUNC_INIT_A, STEP_DET_INIT); //pedo initialization request embedded function


  //i2c_write(ADDRESS, 0x01, 0); //return to normal registers????
}

uint16_t readPedometer(){
  Wire.beginTransmission(ADDRESS);
  Wire.write(EMB_FUNC_STEP_COUNTER_L);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDRESS, 2, true); 
  //uint16_t steps = (Wire.read() << 8 | Wire.read());
  uint16_t steps = (Wire.read() | Wire.read() << 8);
  return steps;
}

void i2c_write(uint8_t addr, uint8_t reg, uint8_t val){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  int status = Wire.endTransmission();
  Serial.println(status);
}