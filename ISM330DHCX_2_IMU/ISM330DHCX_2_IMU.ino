/*
  INTERRUPT-BASED IMU EXAMPLE FOR ISM330

  Please if you use this example during my classes be prepared to answer these questions:
  - What is interrupt?
  - Why variable used in interrupt must be volatile?
  - Why we use interrupt to compute delta time (dt) for proper gyroscope integration?
*/

#include <Adafruit_ISM330DHCX.h>
#include <Adafruit_AHRS.h>

#define MICROS_TO_SEC (1.0e-6)

Adafruit_ISM330DHCX ism;
Adafruit_Mahony filter;

volatile bool isDataReady = false;
unsigned long timestamp = 0;

float offsetX = 0;
float offsetY = 0;
float offsetZ = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  if(!ism.begin_I2C(0x6A)) {
    Serial.println("ISM330DHCX connection error");
  }

  ism.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  ism.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  ism.setAccelDataRate(LSM6DS_RATE_104_HZ);
  ism.setGyroDataRate(LSM6DS_RATE_104_HZ);

  //Use int2 for gyroscope data
  ism.configIntOutputs(false, false);
  ism.configInt2(false, true, false);

  //Reinitialize to fix library issue of not handling correcly interrupt enable in some scenarios.
  //(need further investigation)
  // ism.setGyroDataRate(ism.getGyroDataRate());
  // ism.setGyroRange(ism.getGyroRange()); 
  
  delay(100);
  calibrateGyro(100);

  //fire the interrupt whenever a gyroscope data is available
  attachInterrupt(digitalPinToInterrupt(2), onGyroDataReady, RISING);

  filter.begin(104);
}

void loop() {

  //compute orientation whenever gyroscope data is ready
  if(isDataReady){
    //compute delta time between gyroscope measurements, it is mandatory to do before all computations
    float dt = getDt();

    //get sensor measurements
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    ism.getEvent(&accel, &gyro, &temp);

    //apply calibration
    gyro.gyro.x -= offsetX;
    gyro.gyro.y -= offsetY;
    gyro.gyro.z -= offsetZ;

    //discard noise when standing still
    if(fabs(gyro.gyro.x) < 0.001) gyro.gyro.x = 0;
    if(fabs(gyro.gyro.y) < 0.001) gyro.gyro.y = 0;
    if(fabs(gyro.gyro.z) < 0.001) gyro.gyro.z = 0;

    //mandatory conversion for IMU algorithm
    gyro.gyro.x *= RAD_TO_DEG;
    gyro.gyro.y *= RAD_TO_DEG;
    gyro.gyro.z *= RAD_TO_DEG;

    filter.updateIMU(
      gyro.gyro.x,
      gyro.gyro.y,
      gyro.gyro.z,
      accel.acceleration.x,
      accel.acceleration.y,
      accel.acceleration.z,
      dt
    );

    float w, x, y, z;
    filter.getQuaternion(&w, &x, &y, &z);
   
    /*
      Quaternion output for Adafruit web viewer. 
      https://adafruit.github.io/Adafruit_WebSerial_3DModelViewer/

      Quaternions are more suitable for orientation calculations
      but are difficult to use when comparing angles or doing
      something based on device tilt - in this case use euler angles.

      Most libraries use quaternions internally.

    */
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

    isDataReady = false;
  }
}

void onGyroDataReady(){
  isDataReady = true;
  digitalWrite(13, HIGH);
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

void calibrateGyro(int samples){
  for(int i=0; i<samples; i++){
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    ism.getEvent(&accel, &gyro, &temp);

    offsetX += gyro.gyro.x;
    offsetY += gyro.gyro.y;
    offsetZ += gyro.gyro.z;
    delay(20);
  }

  offsetX /= float(samples);
  offsetY /= float(samples);
  offsetZ /= float(samples);
}