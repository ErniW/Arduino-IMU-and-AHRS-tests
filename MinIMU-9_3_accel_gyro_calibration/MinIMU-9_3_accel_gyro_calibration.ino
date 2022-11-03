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

  float totalAccelX = 0;
  float totalAccelY = 0;
  float totalAccelZ = 0;
  float totalGyroX = 0;
  float totalGyroY = 0;
  float totalGyroZ = 0;

  int samples = 10000;

  Serial.println("Starting Calibration...");
  Serial.println("Do not move the device");
  delay(4000);
  Serial.println("Calibrating...");

  for(int i=0; i < samples; i++){

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    lsm6ds33.getEvent(&accel, &gyro, &temp);

    totalGyroX += gyro.gyro.x;
    totalGyroY += gyro.gyro.y;
    totalGyroZ += gyro.gyro.z;

    totalAccelX += accel.acceleration.x;
    totalAccelY += accel.acceleration.y;
    totalAccelZ += accel.acceleration.z - 9.806;

    delay(1);
  }

  float accelMeanX = totalAccelX / samples;
  float accelMeanY = totalAccelY / samples;
  float accelMeanZ = totalAccelZ / samples;

  float gyroMeanX = totalGyroX / samples;
  float gyroMeanY = totalGyroY / samples;
  float gyroMeanZ = totalGyroZ / samples;


  Serial.println("Accel XYZ offset:");
  Serial.print(accelMeanX);
  Serial.print('\t');
  Serial.print(accelMeanY);
  Serial.print('\t');
  Serial.println(accelMeanZ);

  Serial.println("Gyro XYZ offset:");
  Serial.print(gyroMeanX);
  Serial.print('\t');
  Serial.print(gyroMeanY);
  Serial.print('\t');
  Serial.println(gyroMeanZ);
  Serial.println(' ');
}

void loop() {

}

