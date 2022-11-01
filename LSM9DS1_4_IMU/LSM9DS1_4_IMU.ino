#include <Arduino_LSM9DS1.h>
#include <Adafruit_AHRS.h>

Adafruit_Madgwick filter;

int updateRate = 100;
int printEveryUpdate = 5;

float mag_hardIron[3] = {49.29, -34.35, -58.99};
float mag_softIron[9] = {
  0.995, 0.025, 0.003,
  0.025, 0.989, -0.006,
  0.003, -0.006, 1.017
};
float accel_calibration[3] = {-0.01, 0.06, 0.01};
float gyro_calibration[3] = {-0.55, 0.52, 1.15};

void setup() {
  Serial.begin(115200);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }

  filter.begin(updateRate);
}

unsigned long timestamp = 0;
int counter = 0;

float ax, ay, az, gx, gy, gz, mx, my, mz = 0;

void loop() {

  if(millis() - timestamp >= 1000 / updateRate){  

    if(IMU.accelerationAvailable()) IMU.readAcceleration(ax, ay, az);
    if(IMU.gyroscopeAvailable()) IMU.readGyroscope(gx, gy, gz);
    if(IMU.magneticFieldAvailable()) IMU.readMagneticField(mx, my, mz);

    calibrate(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    //filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
    filter.update(-gy, -gx, gz, -ay, -ax, az, mx, my, mz);

    timestamp = millis();

    counter++;

    if(counter == printEveryUpdate){

      float w, x, y, z;
      filter.getQuaternion(&w,&x,&y,&z);

      Serial.print("Quaternion: ");
      Serial.print(w);
      Serial.print(',');
      Serial.print(x);
      Serial.print(',');
      Serial.print(y);
      Serial.print(',');
      Serial.println(z);

      // float roll = filter.getRoll();
      // float pitch = filter.getPitch();
      // float yaw = filter.getYaw();

      // Serial.print("Orientation: ");
      // Serial.print(yaw);
      // Serial.print(',');
      // Serial.print(pitch);
      // Serial.print(',');
      // Serial.println(roll);

      counter = 0;
    }
  }
}

void calibrate(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz){
  *ax -= accel_calibration[0];
  *ay -= accel_calibration[1];
  *az -= accel_calibration[2];

  *gx -= gyro_calibration[0];
  *gy -= gyro_calibration[1];
  *gz -= gyro_calibration[2];

  *mx -= mag_hardIron[0];
  *my -= mag_hardIron[1];
  *mz -= mag_hardIron[2];

  *mx = *mx * mag_softIron[0] + *my * mag_softIron[1] + *mz * mag_softIron[2];
  *my = *mx * mag_softIron[3] + *my * mag_softIron[4] + *mz * mag_softIron[5];
  *mz = *mx * mag_softIron[6] + *my * mag_softIron[7] + *mz * mag_softIron[8];
}
