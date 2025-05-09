**A series of tests made together with students for the purpose of one of our semester project at Industrial Design faculty. I decided to put it online as reference for future projects that require orientation sensing.**

## Sensors tested:
### 6-dof (Gyroscope + Accelerometer):
- **MPU-6050**
- **ISM330DHCX**

### 9-dof (Gyroscope + Accelerometer + Magnetometer)
- **Pololu MinIMU-9 v5 9DOF** (LSM6DS33 + LIS3MDL)
- **LSM9DS1** in Arduino Nano 33 BLE
- **BNO055** (Adafruit module)
- *ICM-20948 *Not included, tested as 6-dof in 2021*

## Methods of test:
- Tests were made in similar conditions.
- Tests were made with sensor grabbed in hand for basic gestures and then whole device inside enclosure (indirect movement).
- Slow and fast movement.
- Checking edge angles to cause gimbal lock.
- We used similar calibration method for magnetometer. MotionCal is available for download here: https://www.pjrc.com/store/prop_shield.html Accelerometer and Gyro is not working here.
- Testing with Adafruit app. https://adafruit.github.io/Adafruit_WebSerial_3DModelViewer/. Be aware of format how data must be send `Orientation: x,y,z` or `Quaternion: w,x,y,z`.

## Notes:
*I've included some further explanations - Ernest*
- **6-dof IMUs are enough to measure tilt angle. For yaw/Z-axis rotation 9-dof (with magnetometer) or precise gyro calculation is necessary.**
- Always include calibration code.
- **Gyroscope data integration requires matching with defined frequency and updating data when new measurement is available. Period.** *It's difficult to do properly with Arduino libraries. Ideally we should avoid any delays and use interrupts. Without proper handling it just won't work. Depending on libraries it's handled differently. Our projects rarely requires advanced usage. It's an issue if your project requires all three axes or we really have to handle the z-axis.*
- **Integration problems can be easily spotted if we rotate by 90 degrees but the axis moves by different angle and error accumulates over time.** (it integrates too much/few samples per second therefore error grows over time) It's issue with Z-axis which relies on gyroscope (unless we use magnetometer). Tilt measurements are dependent on accelerometer which doesn't drift but have noise in rapid movements.
- **Drifting in gyroscope can be spotted if our axis moves by some small angle over time without any movement.** Technically drifting can't be avoided but we can marginalize it by proper callibration or compensation over time if device is running for hours. Usually libraries handle the compensation with accelerometer.
- **Stick to quaternions in your code for calculations, eventually convert to euler angles to avoid gimbal lock.** *Quaternions are 4 dimensional (w, x, y, z) representations of 3 dimensional rotations and are less intuitive to read but are better for handling continuous movement.*
- *Raw values can be useful to detect if object is shaking, is tapped, moving or tilting toward one of its side.* **Sometimes you don't have to compute the euler angles.** *Furthermore, you can use a simpler complementary filter without involving gyroscope.*
- **Raw values aren't in SI units** *(conversion is necessary for AHRS libraries), through some libraries we get the SI units by default but some of them calculate angles in radians. Therfore, for example in BNO055 code we multiply by 57.2957795 to get degrees. Furthermore, accelerometer values sometimes are described in G. 1G is equal to gravity acceleration approx 9.8m/s^2. The raw value is scaled to match range of intiger depending on what G sensitivity we set. For example: 2G sensitivity will be scaled from -2G - 2G to -32768 - 32767. The 1G, which is gravity should show approx +/-16k on flat surface on one axis. Gyroscope sensitivity works in similar way.*
- *For accelerometer noise can be reduced by averaging values or additional filtering.*
- *We still didn't figure out how automatic magnetometer calibration routine should be done.*
- **Any 9-dof AHRS algorithm is too demanding for Arduino UNO** *Unless it's the only thing you want to do and not be very precise. 6-dof IMU can work fine.*

# Sensors usage:
## MPU-6050
**We are using the commonly used MPU6050 library. The Adafruit equivalent is cumbersome to configure.**
### Calibration:
Open the *MPU6050_2_raw_values_offsets* example. There are two ways to calibrate the sensor:
- Use `CalibrateAccel` and `CalibrateGyro` to do that automatically.
- Hard-coding: Place the device on a flat surface. Run the calibration code from MPU6050 library (*IMU_Zero*). Write down the averaged offsets and use these offsets in our sketch (eventually deleting the auto calibration).
### Angles:
**Digital Motion Processor (DMP)** calculates the euler angles or quaternions directly in sensor without wasting resources on your Arduino. You can use the library's *MPU6050_DMP6_using_DMP_V6v12* but you need to add *`Wire.setWireTimeout(3000, true);`* after *`Wire.setClock(400000);`* to prevent sketch from randomly freezing. *We also prepared a simplified sketch without unnecessary configurations.*

**Adafruit Unified sensors and AHRS library** requires its Adafruit Calibration Library. We found it more difficult to use and measurements are worse than DMP.

### Notes:
- **MPU-6050 is the cheapest among all. Usually cheapest sensors have significantly lower quality but not in this case thanks to DMP. During our classes don't buy the expensive MPU-6050 variants, the cheap one is enough.**
- *For prototyping, in most cases MPU-6050 is enough if you need to measure tilt angle without relative heading (it doesn't have a magnetometer to do so). The MPU-9250 and ICM-20948 have magnetometer included.*
- **DMP computes Euler angles or quaternions without wasting resources of your Arduino.**
- *Using Mahony filter is not even close to quality of DMP.*
- ***ICM-20948** *is a successor to MPU-6050/MPU-9250. Sparkfun has a library that leverages the same DMP algorithm for 9-dof. The problem is product availability, however there are concerns that Invensense doesn't use 9-dof for its DMP. We can't investigate it until we get one.*

## ISM330DHCX
### Calibration:
1. Use **Adafruit sensorlab** *gyro_zerorate_simplecal* to calibrate gyroscope. It won't store the values on EEPROM or SD card. *It takes a long time to compile.*
2. Write the detected offsets into the *ISM330DHCX_2_IMU* code. Remember that values are in rad/s so we have to do multiplication.

### Angles:
We shall use the Adafruit AHRS library. Remember that without the magnetometer only roll and pitch makes sense.

### Notes: 
- *ISM330DHCX is an industrial-grade version of LSM6DSOX with many enhancements.*
- *ISM330DHCX has built-in tilt direction, double tap, pedometer and free falling detection.*
- *ISM330DHCX doesn't contain DMP but has Finite State Machine and Machine Learning Core for gesture recognition. ST provides iNEMO engine AHRS fusion algorithm but unfortunately it seems they removed it.* **Anyway, advanced features aren't possible to use on Arduino platform so it's not worth buying in our case.**
- *Data is less noisy compared to MPU-6050.*
- *It's compatible with Adafruit LSM6DS library. Unfortunately things like pedometer and double tap aren't implemented. I made my own implementation but it requires basic understanding of bare-metal programming.*

## Pololu MinIMU-9 v5 9DOF
**You can use the adafruit library but the i2c address must be changed to 0x6b `lsm6ds33.begin_I2C(0x6b)` and `lis3mdl.begin_I2C(0x1E)`.**

The magnetometer is a separate sensor on this board.

### Calibration:
1. Magnetometer: MotionCal. Upload the calibration code and open the app.
2. Accelerometer and Gyroscope: calibrated via included code.

### Angles:
There are libraries for these sensors from Pololu, Sparkfun and Adafruit.We will stick to Adafruit as we are using Adafruit AHRS library.

### Notes:
- *It's the same sensor you have on Adafruit Feather BLE.*
- *LIS3MDL magnetometer frequency is faster than magnetometer in LSM9DS1.*
- *You can use Adafruit library to use pedometer.*
- *Pololu libraries are very basic, there is an AHRS example but there are more steps to make it running https://github.com/pololu/minimu-9-ahrs-arduino*

## Arduino Nano 33 BLE (LSM9DS1)
### Calibration:
**Because we are using Arduino Nano 33 BLE we had to prepare a custom code to communicate with Motioncal and to calibrate accelerometer and gyroscope (Adafruit library won't detect the sensor). If you are using Adafruit LSM9DS1 (or LSM9DS0) as a standalone module you can use Adafruit Unified Sensor Library**
1. Magnetometer: MotionCal. Upload the calibration code and open the app.
2. Accelerometer and Gyroscope: calibrated via included code.
3. Write these values into *LSM9DS1_4_AHRS*.

### Angles:
We are using a sketch which sends quaternions to Adafruit web app https://adafruit.github.io/Adafruit_WebSerial_3DModelViewer/ You can easily change to euler angles if you whish.

**The position becomes stable and eventually correct after a short time. Achieving zero-drift correct orientation seems impossible during frequent movements.**

**The problem is the Yaw value - try rotating around Z-axis and after a few 360 degree rotations you see how delayed the movement is.** Magnetometer is set to 20hz frequency which is slow for AHRS. We've changed its Output Data Rate to 80hz and fast ODR `LSM9DS1_CTRL_REG1_M` and the performance mode. Documentation https://www.st.com/resource/en/datasheet/lsm9ds1.pdf

**Different frequency/update ratio somehow fixes the issue, check the difference in v2 version.**

Furthermore, LSM9DS1 magnetometer has different direction of X and Y axes. *You can test them on your own but to be honest it's impossible to see it on that app's Bunny which ones are correct.*

**Things that could improve the measurements:**
- Improve calibration, apply something to additionaly reduce noise?
- **Measurement frequency ratio? Read notes on Gyro integration problems.**
- Different AHRS algorithm? Adafruit contains Mahony, Madgwick and NXP Sensor Fusion.

### Notes:
- *Great all-in-one solution if you have to use Bluetooth LE. The Adafruit Feather BLE can be an alternative but I remember that BT worked significantly slower. On the other hand, Feather BLE includes Li-Po battery circuit.*
- *The Arduino LSM9DS1 library is mediocre, it lacks features except getting the data and it doesn't play well with Adafruit Libraries.*
- *LSM9DS1 documentation misleads it has click/double click detection. The sensor has movement and tilt detection available.*

## BNO055
### Calibration:
**Calibration is done internally on startup and adapts on runtime.** You can print calibration status, numbers between 0-3 are calibration grades.

### Angles:
Reading quaternions is recommended by manufacturer. Later you can convert them to euler angles.

### Notes:
- **For demanding projects, to be honest, if you can and your project can afford it, just buy BNO055 (or equivalent) sensor module for Arduino. The real-time calibration and build-in AHRS fusion algorithm saves a lot of headaches and time.** *("Personally I don't understand why students never value their time so they waste it on figuring things too complex for them or buying the cheapest sensor shipped from China wasting one or two classes to get it while price difference isn't worth it at all. It's a problem with any kind of project." - Ernest)*
- Quality of measurements especially matter when you have to frequently change device orienation axis, based on my observation, other sensors mentioned just lose their orientation. However, for basic scenarios there is no reason to buy it.*
- *This particular version doesn't have any double tap, gesture nor pedometer sensor.*
- **Computations are made internally so it doesn't waste Arduino resources.**