#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"
#include "Wire.h"

//Dear students, please be aware that this code heavily uses pointers.
//Before using read about pointers and references.

MPU6050 mpu_1(0x68);
MPU6050 mpu_2(0x69);

typedef struct{
  Quaternion q;
  VectorFloat gravity;
  float yaw;
  float pitch;
  float roll;
  uint8_t buffer[64];
} MPU6050_values;

MPU6050_values mpu_1_values;
MPU6050_values mpu_2_values;

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Wire.setWireTimeout(3000, true);

  Serial.begin(115200);
  while (!Serial); 

  mpuInit(&mpu_1);
  mpuInit(&mpu_2);

  delay(1000);
}

void loop() {
  
  if(mpu_1.dmpGetCurrentFIFOPacket(mpu_1_values.buffer)) {
    mpuGetValues(&mpu_1, &mpu_1_values);

    Serial.print("Sensor 1: ypr\t");
    Serial.print(mpu_1_values.yaw);
    Serial.print("\t");
    Serial.print(mpu_1_values.pitch);
    Serial.print("\t");
    Serial.print(mpu_1_values.roll);
    Serial.println();
  }

  if(mpu_2.dmpGetCurrentFIFOPacket(mpu_2_values.buffer)) {
    mpuGetValues(&mpu_2, &mpu_2_values);

    Serial.print("Sensor 2: ypr\t");
    Serial.print(mpu_2_values.yaw);
    Serial.print("\t");
    Serial.print(mpu_2_values.pitch);
    Serial.print("\t");
    Serial.print(mpu_2_values.roll);
    Serial.println();
  }

}

void mpuInit(MPU6050* device){
  device->initialize();
  device->dmpInitialize();
  device->CalibrateAccel(6);
  device->CalibrateGyro(6);

  //device->PrintActiveOffsets();

  // Serial.print("Testing connection: ");
  // Serial.println(device->testConnection());

  device->setDMPEnabled(true);
}

void mpuGetValues(MPU6050* device, MPU6050_values* values){
    float ypr[3];
    device->dmpGetQuaternion(&(values->q), values->buffer);
    device->dmpGetGravity(&(values->gravity), &(values->q));
    device->dmpGetYawPitchRoll(ypr, &(values->q), &(values->gravity));
    
    values->yaw = ypr[0] * 180 / M_PI;
    values->pitch = ypr[1] * 180 / M_PI;
    values->roll = ypr[2] * 180 / M_PI;
}