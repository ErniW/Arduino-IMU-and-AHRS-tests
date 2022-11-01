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

  Serial.print("Raw:");
  Serial.print(int(ax*8192/9.8)); Serial.print(",");
  Serial.print(int(ay*8192/9.8)); Serial.print(",");
  Serial.print(int(az*8192/9.8)); Serial.print(",");
  Serial.print(int(gx*57.2957795*16)); Serial.print(",");
  Serial.print(int(gy*57.2957795*16)); Serial.print(",");
  Serial.print(int(gz*57.2957795*16)); Serial.print(",");
  Serial.print(int(mx*10)); Serial.print(",");
  Serial.print(int(my*10)); Serial.print(",");
  Serial.print(int(mz*10)); Serial.println("");
}
