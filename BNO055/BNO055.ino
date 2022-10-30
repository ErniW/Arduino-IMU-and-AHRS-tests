#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
  Serial.begin(115200);
  bno.begin();
  bno.setExtCrystalUse(true);
  Wire.setClock(400000);
}

void loop() {
  uint8_t sysCal, gyroCal, accelCal, magCal = 0;
  bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);

  imu::Quaternion quaternions = bno.getQuat();
  imu::Vector<3> euler = quaternions.toEuler();

  float roll = euler.x() * 57.2957795;
  float pitch = euler.y() * 57.2957795;
  float yaw = euler.z() * 57.2957795;

  Serial.print(sysCal);
  Serial.print(gyroCal);
  Serial.print(accelCal);
  Serial.print(magCal);
  Serial.print("\t");

  Serial.print(roll);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.println(yaw);
}
