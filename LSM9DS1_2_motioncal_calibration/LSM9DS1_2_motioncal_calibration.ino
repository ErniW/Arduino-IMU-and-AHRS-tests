#include <Arduino_LSM9DS1.h>

//Requres improvement, it doesn't work properly

void setup() {
  Serial.begin(115200);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }

  delay(1000);
}

void loop() {
  float ax, ay, az, gx, gy, gz, mx, my, mz;

  if(IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()){
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    
    // if(IMU.magneticFieldAvailable())
      IMU.readMagneticField(mx, my, mz);

    Serial.print("Raw:");
    Serial.print(int(-ay * 8192 / 9.80665)); Serial.print(",");
    Serial.print(int(-ax * 8192 / 9.80665)); Serial.print(",");
    Serial.print(int(az * 8192 / 9.80665)); Serial.print(",");
    Serial.print(int(-gy * 32768 / 2000)); Serial.print(",");
    Serial.print(int(-gx * 32768 / 2000)); Serial.print(",");
    Serial.print(int(gz * 32768 / 2000)); Serial.print(",");
    Serial.print(int(mx * (32768 / 400.0))); Serial.print(",");
    Serial.print(int(my * (32768 / 400.0))); Serial.print(",");
    Serial.print(int(mz * (32768 / 400.0)));
    Serial.println();
  }


}
