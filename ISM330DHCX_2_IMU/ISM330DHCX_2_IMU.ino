/*

accel 0.1 -0.16 9.98
gyro 0 / 0.01 -0.01 0

*/


#include <Adafruit_ISM330DHCX.h>
#include <Adafruit_AHRS.h>
#include <Adafruit_Sensor_Calibration.h>

Adafruit_ISM330DHCX ism330dhcx;
Adafruit_Sensor_Calibration_EEPROM calibration;
Adafruit_Mahony filter;

void setup() {
  Serial.begin(115200);

  if(!ism330dhcx.begin_I2C()) {
    Serial.println("ISM330DHCX connection error");
  }

  calibration.accel_zerog[0] = 0.1;
  calibration.accel_zerog[1] = -0.16;
  //calibration.accel_zerog[2] = 9.98;

  calibration.gyro_zerorate[0] = 0.01;
  calibration.gyro_zerorate[1] = -0.01;
  calibration.gyro_zerorate[2] = 0;

  filter.begin(20);
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  ism330dhcx.getEvent(&accel, &gyro, &temp);

  calibration.calibrate(accel);
  calibration.calibrate(gyro);

  filter.updateIMU(
    gyro.gyro.x,
    gyro.gyro.y,
    gyro.gyro.z,
    accel.acceleration.x,
    accel.acceleration.y,
    accel.acceleration.z
  );

  float roll = filter.getRoll();
  float pitch = filter.getPitch();
  float yaw = filter.getYaw();

  Serial.print(roll);
  Serial.print('\t');
  Serial.print(pitch);
  Serial.print('\t');
  Serial.print(yaw);
  Serial.println('\t');

  delay(20);
}
