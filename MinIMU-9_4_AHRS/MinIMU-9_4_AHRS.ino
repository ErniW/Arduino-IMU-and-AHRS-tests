#include <Wire.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_AHRS.h>

#define UPDATE_RATE   104
#define MICROS_TO_SEC (1.0e-6)
#define MS_SQ_TO_G    0.101972

Adafruit_LSM6DS33 lsm6ds33;
Adafruit_LIS3MDL lis3mdl;
Adafruit_Mahony filter;

float mag_hardIron[3] = {3.75, -32.09, 72.93};
float mag_softIron[9] = {
  0.955, 0.008, 0.058,
  0.008, 1.026, -0.009,
  0.058, -0.009, 1.024
};

float offsetX = 0;
float offsetY = 0;
float offsetZ = 0;

unsigned long timestamp = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); 

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

  delay(100);
  calibrateGyro(100);
  filter.begin(UPDATE_RATE);

  timestamp = micros();
}

void loop() {
  if(lsm6ds33.gyroscopeAvailable()){
    float dt = getDt();

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    lsm6ds33.getEvent(&accel, &gyro, &temp);

    gyro.gyro.x -= offsetX;
    gyro.gyro.y -= offsetY;
    gyro.gyro.z -= offsetZ;

    bool isMagReady = lis3mdl.magneticFieldAvailable();
    float mx = 0;
    float my = 0;
    float mz = 0;

    if(isMagReady){
      lis3mdl.readMagneticField(mx, my, mz);
      mx -= mag_hardIron[0];
      my -= mag_hardIron[1];
      mz -= mag_hardIron[2];
      setMagSoftIron(&mx,&my,&mz);
    }

    if(isMagReady){
      filter.update(
        gyro.gyro.x * RAD_TO_DEG, 
        gyro.gyro.y * RAD_TO_DEG, 
        gyro.gyro.z * RAD_TO_DEG, 
        accel.acceleration.x * MS_SQ_TO_G, 
        accel.acceleration.y * MS_SQ_TO_G, 
        accel.acceleration.z * MS_SQ_TO_G, 
        mx, 
        my, 
        mz,
        dt
      );
    }
    else{
      filter.updateIMU(
        gyro.gyro.x * RAD_TO_DEG, 
        gyro.gyro.y * RAD_TO_DEG, 
        gyro.gyro.z * RAD_TO_DEG, 
        accel.acceleration.x * MS_SQ_TO_G, 
        accel.acceleration.y * MS_SQ_TO_G, 
        accel.acceleration.z * MS_SQ_TO_G, 
        dt
      );
    }

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
  }
}

float getDt() {
  unsigned long currentTime = micros();
  unsigned long difference = 0;
  float dt = 0;

  //handle variable overflow
  if (currentTime >= timestamp) {
    difference = currentTime - timestamp;
  } 
  else {
    difference = (0xFFFFFFFFul - timestamp) + currentTime + 1;
  }

  dt = difference * MICROS_TO_SEC;

  timestamp = currentTime;
  return dt;
}

void setMagSoftIron(float *mx, float *my, float *mz){


  float mxTemp = *mx;
  float myTemp = *my;
  float mzTemp = *mz;
  *mx = mxTemp * mag_softIron[0] + myTemp * mag_softIron[1] + mzTemp * mag_softIron[2];
  *my = mxTemp * mag_softIron[3] + myTemp * mag_softIron[4] + mzTemp * mag_softIron[5];
  *mz = mxTemp * mag_softIron[6] + myTemp * mag_softIron[7] + mzTemp * mag_softIron[8];
}

void calibrateGyro(int samples){
  for(int i=0; i<samples; i++){
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    lsm6ds33.getEvent(&accel, &gyro, &temp);

    offsetX += gyro.gyro.x;
    offsetY += gyro.gyro.y;
    offsetZ += gyro.gyro.z;
    delay(20);
  }

  offsetX /= float(samples);
  offsetY /= float(samples);
  offsetZ /= float(samples);
}
