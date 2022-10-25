**A series of tests made together with students for the purpose of one of our semester project at Industrial Design faculty. I decided to put it online as reference for future projects that require orientation sensing.**
## Sensors tested:
### 6-dof:
- **MPU-6050** using raw data and DMP.
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
- *The Arduino LSM9DS1 library is mediocre, it lacks features beside getting the data.*
- *We still didn't figure out how automatic calibration should be done.*

## Some additional features tested:
- Double Tap gesture
- Free fall detection
- ISM330DHCX finite state machine
