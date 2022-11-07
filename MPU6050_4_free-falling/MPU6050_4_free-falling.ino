#include "MPU6050.h"

MPU6050 mpu6050;

void setup() {
  Serial.begin(38400);
  mpu6050.initialize();

  mpu6050.setIntFreefallEnabled(true);
  mpu6050.setIntZeroMotionEnabled(false);
  mpu6050.setIntMotionEnabled(false);

  mpu6050.setAccelerometerPowerOnDelay(3);
  mpu6050.setDHPFMode(MPU6050_IMU::MPU6050_DHPF_5);

  mpu6050.setFreefallDetectionThreshold(17);
  mpu6050.setFreefallDetectionDuration(2);	

  attachInterrupt(digitalPinToInterrupt(2), freeFallInt, RISING);
}

void loop() {

}

void freeFallInt(){
  Serial.println("Free falling!");
}