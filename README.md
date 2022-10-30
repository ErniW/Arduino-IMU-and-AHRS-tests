**A series of tests made together with students for the purpose of one of our semester project at Industrial Design faculty. I decided to put it online as reference for future projects that require orientation sensing.**
## Sensors tested:
### 6-dof:
- **MPU-6050**
- **ISM330DHCX**

### 9-dof
- **Pololu MinIMU-9 v5 9DOF** (LSM6DS33 + LIS3MDL)
- **Arduino Nano 33 BLE** (built-in LSM9DS1)
- **BNO055*** (Adafruit module)

### Not included 9-dof:
*Tested in 2021*
- ICM-20948* 

## Methods of test:
- We were testing the devices for IMU and full AHRS with magnetometer.
- We tried to test in similar conditions.
- We used similar calibration method for magnetometer.

## Notes:

- ***ICM-20948** *is a successor to MPU-6050/MPU-9250. Sparkfun has a library that leverages the same DMP algorithm. The problem is product availability.*
- *Stick to quaternions in your code for calculations, convert to euler angles at the very end to avoid gimbal lock.*
- *Check your calibration whenever the sensor is drifting when values changes without moving the device. However, remember that 6-dof Z-axis will never be correct comparing to 9-dof.*
- *Raw values can be useful to detect if object is shaking, moving or tilting toward one of its side.* **Sometimes you don't have to compute the euler angles.**
- *We still didn't figure out how automatic calibration should be done.*

## Some additional features tested:
- Double Tap gesture
- Free fall detection
- ISM330DHCX finite state machine

# Sensors usage:
## MPU-6050
**We are using the commonly used MPU6050 library. The Adafruit equivalent is cumbersome to configure.**
### Calibration:
Open the *MPU6050_2_raw_values_offsets* example. There are two ways to calibrate the sensor:
- Use `CalibrateAccel` and `CalibrateGyro` to do that automatically.
- Hard-coding: Place the device on a flat surface. Run the calibration code from MPU6050 library (*IMU_Zero*). Write down the averaged offsets and use these offsets (eventually deleting the auto calibration).
### Angles:
**Digital Motion Processor (DMP)** calculates the euler angles or quaternions directly in sensor without wasting resources on your Arduino. You can use the library's *MPU6050_DMP6_using_DMP_V6v12* but you need to add *`Wire.setWireTimeout(3000, true);`* after *`Wire.setClock(400000);`* to prevent sketch from randomly freezing. *We also prepared a simplified sketch without unnecessary configurations.*

**Adafruit Unified sensors and AHRS library** requires its Adafruit Calibration Library. We found it more difficult to use and measurements are worse than DMP.

### Notes:
- *MPU-6050 is the cheapest among all. Usually cheapest sensors have significantly lower quality but not in this case thanks to DMP.*
- *For prototyping, in most cases MPU-6050 is enough if you need to measure tilt angle without relative heading (it doesn't have a magnetometer to do so). The MPU-9250 and ICM-20948 have magnetometer included.*
- *DMP computes Euler angles or quaternions without wasting resources of your Arduino.*
- *Using Mahony filter is not even close to quality of DMP.*

## ISM330DHCX
### Calibration:
1. 

### Angles:

### Notes: 
- *ISM330DHCX is an industrial-grade version of LSM6DSOX with improved durability and gyro range.*
- *ISM330DHCX has built-in tilt side direction, double tap, pedometer and free falling detection.*
- *ISM330DHCX doesn't contain DMP but has Finite State Machine for machine learning and gesture recognition. For AHRS fusion algorithm STM provides iNEMO engine software. Unfortunately it seems they removed it. These features aren't possible to use on Arduino platform.*

## Pololu MinIMU-9 v5 9DOF
### Calibration:
- Magnetometer calibration is done via MotionCal. Upload the calibration code and open the app.

### Angles:
There are various kinds of libraries,

## Arduino Nano 33 BLE
*The Arduino LSM9DS1 library is mediocre, it lacks features beside getting the data.*

## BNO055
### Calibration:
Calibration is done internally on startup. You can print calibration status, numbers between 0-3 are calibration levels.

### Angles:
Reading quaternions is recommended by manufacturer. Later you can convert them to euler angles.

### Notes:
- **For demanding projects, to be honest, if you can and your project can afford it, just buy a Bosch* **BNO055** *(or equivalent) sensor module for Arduino. The real-time calibration and build-in AHRS fusion algorithm saves a lot of headaches and time. Especially it does matter when you have to frequently change device orienation axis, based on my observation, other sensors mentioned just lose their orientation. However, for basic scenarios there is no reason to buy it.*
- *This particular version doesn't have any double tap, gesture nor pedometer sensor.*