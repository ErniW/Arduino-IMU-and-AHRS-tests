#include "MPU6050.h"

MPU6050 mpu6050;

int ax, ay, az;
int gx, gy, gz;

void setup() {
  Serial.begin(38400);
  mpu6050.initialize();

  // mpu6050.setXAccelOffset(-2020);
  // mpu6050.setYAccelOffset(-1482);
  // mpu6050.setZAccelOffset(608);
  // mpu6050.setXGyroOffset(145);
  // mpu6050.setYGyroOffset(-37);
  // mpu6050.setZGyroOffset(-31);

  mpu6050.CalibrateAccel(6);
  mpu6050.CalibrateGyro(6);
}

void loop() {

  mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("Accel XYZ:");
  Serial.print("\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");

  Serial.print("Gyro XYZ:");
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.println(gz);
  delay(80);
}
