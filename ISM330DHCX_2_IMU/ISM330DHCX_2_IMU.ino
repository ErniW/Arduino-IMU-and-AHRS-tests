#include <Adafruit_ISM330DHCX.h>
#include <Adafruit_AHRS.h>

Adafruit_ISM330DHCX ism;
Adafruit_Mahony filter;

void setup() {
  Serial.begin(115200);

  if(!ism.begin_I2C(0x6A)) {
    Serial.println("ISM330DHCX connection error");
  }

  filter.begin(20);
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  ism.getEvent(&accel, &gyro, &temp);

  gyro.gyro.x -= -0.0024 * 57.2957795;
  gyro.gyro.y -= -0.0098 * 57.2957795;
  gyro.gyro.z -= 0.0024 * 57.2957795;

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

  Serial.print("Orientation: ");
  Serial.print(roll);
  Serial.print('\t');
  Serial.print(pitch);
  Serial.print('\t');
  Serial.print(yaw);
  Serial.println('\t');
}