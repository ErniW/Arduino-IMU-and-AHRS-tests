#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"
#include "Wire.h"

typedef struct{
  Quaternion qat;
  VectorFloat gravity;
  float yaw;
  float pitch;
  float roll;
} MPU6050_values;

MPU6050_values mpu_1_values;
MPU6050_values mpu_2_values;

//To set secondary address set pin AD0 to high, otherwise both will have address conflict.
MPU6050 mpu_1(0x68);
MPU6050 mpu_2(0x69);

//Both devices share same buffer. If for whatever reason you've decided
//to use RTOS or interrupts you should create separate buffer per device
//and avoid overwriting values. Current code is safe because it parses
//values from buffer immediately.
uint8_t dataBuffer[64];

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Wire.setWireTimeout(3000, true);

  Serial.begin(115200);
  while (!Serial); 

  Serial.println("\nSensor 1 Init:");
  mpuInit(mpu_1);

  Serial.println("\nSensor 2 Init:");
  mpuInit(mpu_2);
  

  delay(1000);
}

void loop() {
  
  if(mpu_1.dmpGetCurrentFIFOPacket(dataBuffer)) {
    mpuGetValues(mpu_1, mpu_1_values);

    Serial.print("Sensor 1 ypr: \t");
    Serial.print(mpu_1_values.yaw);
    Serial.print("\t");
    Serial.print(mpu_1_values.pitch);
    Serial.print("\t");
    Serial.print(mpu_1_values.roll);
    Serial.println();
  }

  if(mpu_2.dmpGetCurrentFIFOPacket(dataBuffer)) {
    mpuGetValues(mpu_2, mpu_2_values);

    Serial.print("Sensor 2 ypr:\t");
    Serial.print(mpu_2_values.yaw);
    Serial.print("\t");
    Serial.print(mpu_2_values.pitch);
    Serial.print("\t");
    Serial.print(mpu_2_values.roll);
    Serial.println();
  }

}

void mpuInit(MPU6050& device){
  device.initialize();
  device.dmpInitialize();
  device.CalibrateAccel(6);
  device.CalibrateGyro(6);

  //device.PrintActiveOffsets();

  // Serial.print("Testing connection: ");
  // Serial.println(device.testConnection());

  device.setDMPEnabled(true);
}

void mpuGetValues(MPU6050& device, MPU6050_values& values){
    float ypr[3];
    device.dmpGetQuaternion(&values.qat, dataBuffer);
    device.dmpGetGravity(&values.gravity, &values.qat);
    device.dmpGetYawPitchRoll(ypr, &values.qat, &values.gravity);
    
    values.yaw = ypr[0] * 180 / M_PI;
    values.pitch = ypr[1] * 180 / M_PI;
    values.roll = ypr[2] * 180 / M_PI;
}