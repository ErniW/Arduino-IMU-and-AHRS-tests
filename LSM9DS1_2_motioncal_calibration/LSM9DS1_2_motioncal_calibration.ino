#include <Arduino_LSM9DS1.h>

//Requres improvement, it doesn't work properly

#define LSM9DS1_ADDRESS     0x6b
#define LSM9DS1_ADDRESS_M   0x1e

#define LSM9DS1_OUT_X_G     0x18
#define LSM9DS1_OUT_X_XL    0x28
#define LSM9DS1_OUT_X_L_M   0x28

#define MAG_FREQ_80HZ (((1 << 2) | (1 << 3) | (1 << 4))  << 2)

void setup() {
  Serial.begin(115200);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }

  Wire1.beginTransmission(0x1e);
  Wire1.write(0x20);
  Wire1.write(MAG_FREQ_80HZ);
  Wire1.endTransmission();

  delay(1000);
}

void loop() {
  // float ax, ay, az, gx, gy, gz, mx, my, mz;

  if(IMU.gyroscopeAvailable()){
    // IMU.readAcceleration(ax, ay, az);
    // IMU.readGyroscope(gx, gy, gz);
    
    // if(IMU.magneticFieldAvailable())
    //   IMU.readMagneticField(mx, my, mz);


    //read gyroscope data
    Wire1.beginTransmission(LSM9DS1_ADDRESS);
    Wire1.write(LSM9DS1_OUT_X_G);
    Wire1.endTransmission(false);
    Wire1.requestFrom(LSM9DS1_ADDRESS, 6);

    int16_t gx = (Wire1.read() | Wire1.read() << 8);
    int16_t gy = (Wire1.read() | Wire1.read() << 8);
    int16_t gz = (Wire1.read() | Wire1.read() << 8);

    //read accel data
    Wire1.beginTransmission(LSM9DS1_ADDRESS);
    Wire1.write(LSM9DS1_OUT_X_XL);
    Wire1.endTransmission(false);
    Wire1.requestFrom(LSM9DS1_ADDRESS, 6);

    int16_t ax = (Wire1.read() | Wire1.read() << 8);
    int16_t ay = (Wire1.read() | Wire1.read() << 8);
    int16_t az = (Wire1.read() | Wire1.read() << 8);

    //read mag data
    Wire1.beginTransmission(LSM9DS1_ADDRESS_M);
    Wire1.write(LSM9DS1_OUT_X_L_M);
    Wire1.endTransmission(false);
    Wire1.requestFrom(LSM9DS1_ADDRESS_M, 6);

    int16_t mx = (Wire1.read() | Wire1.read() << 8);
    int16_t my = (Wire1.read() | Wire1.read() << 8);
    int16_t mz = (Wire1.read() | Wire1.read() << 8);

    Serial.print("Raw:");
    Serial.print(-ay); Serial.print(",");
    Serial.print(-ax); Serial.print(",");
    Serial.print(az); Serial.print(",");
    Serial.print(-gy); Serial.print(",");
    Serial.print(-gx); Serial.print(",");
    Serial.print(gz); Serial.print(",");
    Serial.print(mx); Serial.print(",");
    Serial.print(my); Serial.print(",");
    Serial.print(mz); Serial.println();
  }


}
