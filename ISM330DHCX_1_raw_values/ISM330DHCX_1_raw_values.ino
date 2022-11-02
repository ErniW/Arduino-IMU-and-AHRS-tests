#include <Adafruit_ISM330DHCX.h>

Adafruit_ISM330DHCX ism;

void setup() {
  Serial.begin(115200);

  if(!ism.begin_I2C()) {
    Serial.println("ISM330DHCX connection error");
  }
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  ism.getEvent(&accel, &gyro, &temp);

  Serial.print("Accel XYZ: ");
  Serial.print("\t");
  Serial.print(accel.acceleration.x);
  Serial.print("\t");
  Serial.print(accel.acceleration.y);
  Serial.print("\t");
  Serial.print(accel.acceleration.z);

  Serial.print("\tGyro XYZ: ");
  Serial.print("\t");
  Serial.print(gyro.gyro.x);
  Serial.print("\t");
  Serial.print(gyro.gyro.y);
  Serial.print("\t");
  Serial.println(gyro.gyro.z);

  delay(20);
}
