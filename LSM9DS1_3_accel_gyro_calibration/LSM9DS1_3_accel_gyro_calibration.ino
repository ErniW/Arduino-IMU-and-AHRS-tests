#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }

  float totalAccelX = 0;
  float totalAccelY = 0;
  float totalAccelZ = 0;
  float totalGyroX = 0;
  float totalGyroY = 0;
  float totalGyroZ = 0;

  int samples = 10000;

  Serial.println("Starting Calibration...");
  delay(4000);
  Serial.println("Calibrating...");

  for(int i=0; i < samples; i++){
    float ax, ay, az;
    IMU.readAcceleration(ax, ay, az);

    totalAccelX += ax;
    totalAccelY += ay;
    totalAccelZ += az - 0.98;

    float gx, gy, gz;
    IMU.readGyroscope(gx, gy, gz);

    totalGyroX += gx;
    totalGyroY += gy;
    totalGyroZ += gz;

    delay(1);
  }

  float accelMeanX = totalAccelX / samples;
  float accelMeanY = totalAccelY / samples;
  float accelMeanZ = totalAccelZ / samples;

  float gyroMeanX = totalGyroX / samples;
  float gyroMeanY = totalGyroY / samples;
  float gyroMeanZ = totalGyroZ / samples;


  Serial.println("Accel XYZ offset:");
  Serial.print(accelMeanX);
  Serial.print('\t');
  Serial.print(accelMeanY);
  Serial.print('\t');
  Serial.println(accelMeanZ);

  Serial.println("Gyro XYZ offset:");
  Serial.print(gyroMeanX);
  Serial.print('\t');
  Serial.print(gyroMeanY);
  Serial.print('\t');
  Serial.println(gyroMeanZ);
  Serial.println(' ');
}

void loop() {

}
