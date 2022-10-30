**A series of tests made together with students for the purpose of one of our semester project at Industrial Design faculty. I decided to put it online as reference for future projects that require orientation sensing.**
## Sensors tested:
### 6-dof:
- **MPU-6050**
- **ISM330DHCX**

### 9-dof
- **Pololu MinIMU-9 v5 9DOF** (LSM6DS33 + LIS3MDL)
- **Arduino Nano 33 BLE** (built-in LSM9DS1)
- BNO055*

## Methods of test:
- We were testing the devices for IMU and full AHRS with magnetometer.
- We tried to test in similar conditions.
- We used similar calibration method for magnetometer.

## Notes:
- **For demanding projects, to be honest, if you can and your project can afford it, just buy a Bosch* **BNO055** *(or something equivalent) sensor module for Arduino. The real-time calibration and build-in AHRS fusion algorithm saves a lot of headaches and time. It does matter when you have to regularly change device orienation axis, other sensors mentioned just lose their orientation. Probably you could achieve similar results without such feature eventually writing your code from scratch but you probably wouldn't have to read this if you know how to deal with coding and math equations. I've made one project with it but I had to give it to someone with whom I've collaborated so I couldn't include it in this test.*
- *Stick to quaternions in your code for calculations, convert to euler angles at the very end to avoid gimbal lock.*
- *Whenever the sensor is drifting when values changes without moving the device. Check your calibration.*
- *Raw values can be useful to detect if object is shaking, moving or tilting toward one of its side. Sometimes you don't have to compute the euler angles.*
- *The Arduino LSM9DS1 library is mediocre, it lacks features beside getting the data.*
- *We still didn't figure out how automatic calibration should be done.*

## Some additional features tested:
- Double Tap gesture
- Free fall detection
- ISM330DHCX finite state machine

# Sensors usage:
## MPU-6050
**We are using the commonly used MPU6050 library. The Adafruit equivalent doesn't support certain features and is not directly compatible with Adafruit unified sensors.**
### Test:
Run the raw values sketch to check if the sensor is working.
### Calibration:
Open the *MPU6050_2_raw_values_offsets* example. There are two ways to calibrate the sensor:
- Use `CalibrateAccel` and `CalibrateGyro` to do that automatically.
- Place the device on a flat surface. Run the calibration code from MPU6050 library (*IMU_Zero*). Write down the averaged offsets and use these offsets (deleting the auto calibration).
### Angles:
**Digital Motion Processor (DMP)** calculates the euler angles or quaternions directly in sensor without wasting resources on your Arduino. You can use the library's *MPU6050_DMP6_using_DMP_V6v12* but you need to add *`Wire.setWireTimeout(3000, true);`* after *`Wire.setClock(400000);`* to prevent sketch from randomly freezing. *We also prepared a simplified sketch without unnecessary configurations.*

### Notes:
- *MPU-6050 is the cheapest among all. Usually cheapest sensors have significantly lower quality but not in this case thanks to DMP.*
- *For prototyping, in most cases MPU-6050 is enough if you need to measure tilt angle without relative heading (it doesn't have a magnetometer to do so).
- *DMP computes Euler angles or quaternions without wasting resources of your Arduino.*
- *Using Mahony filter is not even close to quality of DMP.*

### ISM330DHCX
### Pololu MinIMU-9 v5 9DOF
### Arduino Nano 33 BLE