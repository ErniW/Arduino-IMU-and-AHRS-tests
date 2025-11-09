#include <Adafruit_LSM9DS1.h>

Adafruit_LSM9DS1 lsm(&Wire1, 0x6b);

void setup(){
  Serial.begin(115200);

  lsm.begin();

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G, lsm.LSM9DS1_ACCELDATARATE_119HZ);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

void loop(){
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sensors_event_t mag;
  lsm.getEvent(&accel, &mag, &gyro, &temp );

  Serial.print("Raw:");
  Serial.print(int(accel.acceleration.x*8192/9.8)); Serial.print(",");
  Serial.print(int(accel.acceleration.y*8192/9.8)); Serial.print(",");
  Serial.print(int(accel.acceleration.z*8192/9.8)); Serial.print(",");
  Serial.print(int(gyro.gyro.x*57.2957795*16)); Serial.print(",");
  Serial.print(int(gyro.gyro.y*57.2957795*16)); Serial.print(",");
  Serial.print(int(gyro.gyro.z*57.2957795*16)); Serial.print(",");
  Serial.print(int(mag.magnetic.x*10)); Serial.print(",");
  Serial.print(int(mag.magnetic.y*10)); Serial.print(",");
  Serial.print(int(mag.magnetic.z*10)); Serial.println("");

  //Adafruit library for LSM9DS1 doesn't have data available function?
  //We match to 20 Hz magnetometer data
  delay(50);
}

