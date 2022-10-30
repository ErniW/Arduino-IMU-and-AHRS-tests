#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"
#include "Wire.h"

MPU6050 mpu;

uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Wire.setWireTimeout(3000, true);

  Serial.begin(115200);
  while (!Serial); 

  mpu.initialize();
  mpu.dmpInitialize();

  mpu.CalibrateAccel(6);
  mpu.CalibrateGyro(6);
  mpu.PrintActiveOffsets();
  
  mpu.setDMPEnabled(true);
}

void loop() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / M_PI);
    Serial.println();
  }

}
