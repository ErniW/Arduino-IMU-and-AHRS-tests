#include <Wire.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>

Adafruit_LSM6DS33 lsm6ds33;
Adafruit_LIS3MDL lis3mdl;

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10); 

  if (!lsm6ds33.begin_I2C(0x6B)) {
    Serial.println("LSM6DS connection error");
    while(1);
  }

  if (!lis3mdl.begin_I2C(0x1E)) {
    Serial.println("LIS3MDL connection error");
    while(1);
  }

  lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds33.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds33.setGyroDataRate(LSM6DS_RATE_104_HZ);

  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sensors_event_t mag; 

  lsm6ds33.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);

  Serial.print("Accel: ");
  Serial.print(accel.acceleration.x);
  Serial.print('\t');
  Serial.print(accel.acceleration.y);
  Serial.print('\t');
  Serial.print(accel.acceleration.z);
  Serial.print(" m/s^2 ");

  Serial.print("\tGyro: ");
  Serial.print(gyro.gyro.x);
  Serial.print('\t');
  Serial.print(gyro.gyro.y);
  Serial.print('\t');
  Serial.print(gyro.gyro.z);
  Serial.print(" radians/s ");

  Serial.print("\tMag: ");
  Serial.print(mag.magnetic.x);
  Serial.print('\t');
  Serial.print(mag.magnetic.y); 
  Serial.print('\t');
  Serial.print(mag.magnetic.z); 
  Serial.println(" uTesla ");

  delay(50);
}
