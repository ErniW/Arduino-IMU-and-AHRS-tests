#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"
#include "Wire.h"

MPU6050 mpu(0x68);
MPU6050 mpu2(0x69);

uint8_t fifoBuffer[64];
uint8_t fifoBuffer2[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

Quaternion q2;
VectorFloat gravity2;
float ypr2[3];

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

  Serial.print(" Testing connection 1: ");
  Serial.println(mpu.testConnection());
  //mpu.PrintActiveOffsets();
  
  mpu.setDMPEnabled(true);

  mpu2.initialize();
  mpu2.dmpInitialize();
  mpu2.CalibrateAccel(6);
  mpu2.CalibrateGyro(6);
  
  Serial.print(" Testing connection 2: ");
  Serial.println(mpu2.testConnection());
  //mpu2.PrintActiveOffsets();

  mpu2.setDMPEnabled(true);

  delay(1000);
}

void loop() {
  
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("Sensor 1: ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / M_PI);
    Serial.println();
  }

  if (mpu2.dmpGetCurrentFIFOPacket(fifoBuffer2)) {
    mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
    mpu2.dmpGetGravity(&gravity2, &q2);
    mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);
    Serial.print("Sensor 2: ypr\t");
    Serial.print(ypr2[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr2[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr2[2] * 180 / M_PI);
    Serial.println();
  }

}
