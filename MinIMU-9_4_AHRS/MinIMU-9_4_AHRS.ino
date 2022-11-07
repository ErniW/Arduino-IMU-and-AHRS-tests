#include <Wire.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_AHRS.h>

Adafruit_LSM6DS33 lsm6ds33;
Adafruit_LIS3MDL lis3mdl;

Adafruit_Mahony filter;

int updateRate = 50;
int printEveryUpdate = 3;

unsigned long timestamp = 0;
int counter = 0;

float mag_hardIron[3] = {-3.59, -23.27, 74.52};
float mag_softIron[9] = {
  0.969, 0.015, 0.038,
  0.015, 1.022, -0.016,
  0.038, -0.016, 1.012
};
float accel_calibration[3] = {-0.33, -0.61, 0.16};
float gyro_calibration[3] = {0.05, -0.06, -0.08};

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10); 

  if (!lsm6ds33.begin_I2C(0x6B)) {
    Serial.println("LSM6DS connection error");
    while(1);
  }

  if (!lis3mdl.begin_I2C(0x1E)) {
    Serial.println("LIS3MDL connection error");
    while(1);
  }

  lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds33.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds33.setGyroDataRate(LSM6DS_RATE_104_HZ);

  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_80_HZ);

  filter.begin(updateRate);
}

float ax, ay, az, gx, gy, gz, mx, my, mz = 0;
sensors_event_t accel;
sensors_event_t gyro;
sensors_event_t temp;
sensors_event_t mag; 

void loop() {
  if(millis() - timestamp >= 1000 / updateRate){  

    lsm6ds33.getEvent(&accel, &gyro, &temp);
    lis3mdl.getEvent(&mag);

    ax = accel.acceleration.x;
    ay = accel.acceleration.y;
    az = accel.acceleration.z;
    gx = gyro.gyro.x;
    gy = gyro.gyro.y;
    gz = gyro.gyro.z;
    mx = mag.magnetic.x;
    my = mag.magnetic.y;
    mz = mag.magnetic.z;

    calibrate(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    filter.update(
      gx * 57.2957795, 
      gy * 57.2957795, 
      gz * 57.2957795, 
      ax * 0.101972, 
      ay * 0.101972, 
      az * 0.101972, 
      mx, 
      my, 
      mz
    );

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
