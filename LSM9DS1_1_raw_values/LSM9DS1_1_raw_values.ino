#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(115200);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }
}

void loop() {
  float ax, ay, az, gx, gy, gz, mx, my, mz;

  IMU.readAcceleration(ax, ay, az);
  IMU.readGyroscope(gx, gy, gz);
  IMU.readMagneticField(mx, my, mz);

  Serial.print(ax);
  Serial.print('\t');
  Serial.print(ay);
  Serial.print('\t');
  Serial.print(az);

  Serial.print('\t');
  Serial.print(gx);
  Serial.print('\t');
  Serial.print(gy);
  Serial.print('\t');
  Serial.print(gz);

  Serial.print('\t');
  Serial.print(mx);
  Serial.print('\t');
  Serial.print(my);
  Serial.print('\t');
  Serial.println(mz);
}
