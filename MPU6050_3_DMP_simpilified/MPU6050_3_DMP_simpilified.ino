/*
  MPU-6050 3d model preview

  https://adafruit.github.io/Adafruit_WebSerial_3DModelViewer/

  This app uses WebSerial which allow us to communicate Arduino
  with an app build like a website.
*/

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"
#include "Wire.h"

MPU6050 mpu;

uint8_t fifoBuffer[64];

Quaternion quat;
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
    mpu.dmpGetQuaternion(&quat, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &quat);
    mpu.dmpGetYawPitchRoll(ypr, &quat, &gravity);

    float yaw = ypr[0] * 180 / M_PI;
    float pitch = ypr[1] * 180 / M_PI;
    float roll = ypr[2] * 180 / M_PI;

    //Output orientation (euler)
    Serial.print("Orientation: ");
    Serial.print(yaw);
    Serial.print(',');
    Serial.println(pitch);
    Serial.print(',');
    Serial.println(roll);

    // Output quaternions
    // Serial.print("Quaternion: ");
    // Serial.print(quat.w);
    // Serial.print(',');
    // Serial.print(quat.x);
    // Serial.print(',');
    // Serial.print(quat.y);
    // Serial.print(',');
    // Serial.println(quat.z);
  }

}
