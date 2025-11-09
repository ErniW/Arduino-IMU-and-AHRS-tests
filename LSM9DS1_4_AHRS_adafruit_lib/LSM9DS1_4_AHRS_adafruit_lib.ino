#include <Arduino_LSM9DS1.h>
#include <Adafruit_AHRS.h>

#define UPDATE_RATE   119
#define MICROS_TO_SEC (1.0e-6)

Adafruit_Madgwick filter;

unsigned long timestamp = 0;

float mag_hardIron[3] = {55.21, -50.13, -68.45};
float mag_softIron[9] = {
  0.992, 0.020, 0.018,
  0.020, 1.002, -0.017,
  0.018, -0.017, 1.007
};

float accel_calibration[3] = {-0.01, 0.06, 0.01};
float gyro_calibration[3] = {0};

void setup() {
  Serial.begin(115200);

  if(!IMU.begin()){
    Serial.println("LSM9DS1 initialization error");
    while(1);
  }

  delay(1000);
  calibrateGyro(100);

  filter.begin(UPDATE_RATE);
}

void loop() {

  if(IMU.gyroscopeAvailable()){
    float dt = getDt();

    float ax, ay, az, gx, gy, gz, mx, my, mz;

    IMU.readGyroscope(gx, gy, gz);
    gx -= gyro_calibration[0];
    gy -= gyro_calibration[1];
    gz -= gyro_calibration[2];

    IMU.readAcceleration(ax, ay, az);
    
    if(IMU.magneticFieldAvailable()){
      IMU.readMagneticField(mx, my, mz);
      mx -= mag_hardIron[0];
      my -= mag_hardIron[1];
      mz -= mag_hardIron[2];

      setMagSoftIron(&mx, &my, &mz);

      filter.update(-gy,-gx,gz,-ay,-ax,az,mx,my,mz, dt);
    }
    else{
      filter.updateIMU(-gy,-gx,gz,-ay,-ax,az, dt);
    }

    float w, x, y, z;
    filter.getQuaternion(&w, &x, &y, &z);

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
    float gx, gy, gz;

    IMU.readGyroscope(gx, gy, gz);

    gyro_calibration[0] += gx;
    gyro_calibration[1] += gy;
    gyro_calibration[2] += gz;
    delay(20);
  }

  gyro_calibration[0] /= float(samples);
  gyro_calibration[1] /= float(samples);
  gyro_calibration[2] /= float(samples);
}
