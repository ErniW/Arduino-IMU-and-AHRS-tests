#include "MPU6050.h"

MPU6050 mpu6050;

/*
Basic measurement offsets: -2024,  -1476,  624 | 155,  -37,  -34
Advanced method: [-2025-2024], [-1477, -1476], [625,625] | [154,154]. [-37,-36], [-34,-33]

*/

int ax, ay, az;
int gx, gy, gz;

void setup() {
  Serial.begin(38400);
  mpu6050.initialize();
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
