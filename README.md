**A series of tests made together with students for the purpose of one of our semester project at Industrial Design faculty. I decided to put it online as reference for future projects that require orientation sensing.**

## Sensors tested:
### 6-dof (Gyroscope + Accelerometer):
- **MPU-6050**
- **ISM330DHCX**

### 9-dof (Gyroscope + Accelerometer + Magnetometer)
- **Pololu MinIMU-9 v5 9DOF** (LSM6DS33 + LIS3MDL)
- **LSM9DS1** in Arduino Nano 33 BLE (temporaily deprecated)
- **BNO055** (Adafruit module)
- *ICM-20948 *Not included, tested as 6-dof in 2021*

## Methods of test:
- Tests were made in similar conditions.
- Tests were made with sensor grabbed in hand for basic gestures and then whole device inside enclosure (indirect movement).
- Slow and fast movement.
- Checking edge angles to cause gimbal lock.
- We used same calibration method for magnetometer. MotionCal is available for download here: https://www.pjrc.com/store/prop_shield.html Accelerometer and Gyro is not working here.
- Tested with Adafruit app. https://adafruit.github.io/Adafruit_WebSerial_3DModelViewer/. Be aware of format how data must be send `Orientation: x,y,z` or `Quaternion: w,x,y,z`.

## Notes:
### Gyroscope & timing:
- **Gyroscope data integration requires matching with defined frequency and updating data when new measurement is available. Period. Interrupt-based delta time measurement yields better results than fixed timing.** *Unfortunately it requires extra effort on Arduino libraries which are very limited on this thing. Our projects never required such precision but ideally, use interrupts to immediately catch available gyroscope data. Secondly, measure delta t with internal timer instead of fixed period (don't lose time by rounding fractions!). Accelerometer doesn't need integration.*
- **Integration problems can be easily spotted if we rotate by 90 degrees but the axis moves by different angle and error accumulates over time.** Error grows over time because we integrate too much/few samples per second therefore. 
- **Drift in gyroscope can be spotted if our axes move by some small angle over time without any movement.** Technically drift can't be avoided but we can marginalize it by proper callibration along with accelerometer compensation. Furthermore, including a small threshold to avoid noise on still object really does its job. Eventually use Kalman filter. Magnetometer can be used for long-term compensation.
- **Because of strict timing use millis() instead of delay() and micros() to compute delta time (to not lose fractions).**
### Units and conversion:
- **Stick to quaternions in your code for calculations, eventually convert to euler angles at the end of computation to avoid gimbal lock.** *Quaternions are 4 dimensional (w, x, y, z) representations of 3 dimensional rotations and are less intuitive to read but are better for handling continuous movement.*
- Arduino libraries are inconsistent so whenever you mix libraries, **check what SI units they operate and what units are expected by your algorithm.** *Some device libraries return raw values, others return SI units by default. Usually it's a conversion between radians and degrees per second.*
- To get SI unit we multiply raw value by its sensitivity. Usually done by library.
### Magnetometer:
- *To be honest, there is a huge misunderstanding on how magnetometer can be used for Arduino projects. The only scenario during our classes is requirement for common heading between at least two objects. It's used for navigation related projects which we don't do. Using it properly is difficult.*
- *We still didn't figure out how automatic magnetometer calibration routine should be done so we use motioncal.*
### Misc:
- Always include calibration code. Some examples directly includes gyroscope calibration. After each start of sketch, give it some time to calibrate.
- For tap/shake detection or simple tilting accelerometer is enough.
- Arduino UNO R3 might not be sufficient in terms of performance and available memory. *Unless there is not much else going on there, eventually MPU6050 doesn't waste Arduino resources.*

# Sensors usage:
## MPU-6050
**We are using the commonly used MPU6050 library and DMP. For our classes this sensor is enough as we almost never need absolute heading of 9-DOF systems.**
### Calibration:
Open the *MPU6050_2_raw_values_offsets* example. There are two ways to calibrate the sensor:
- Use `CalibrateAccel` and `CalibrateGyro` to do that automatically.
- Hard-coding: Place the device on a flat surface. Run the calibration code from MPU6050 library (*IMU_Zero*). Write down the averaged offsets and use these offsets in our sketch (eventually deleting the auto calibration).
### Orientation:
**Digital Motion Processor (DMP)** calculates the euler angles or quaternions directly in sensor without wasting resources on your Arduino. Use the library's *MPU6050_DMP6_using_DMP_V6v12* but you need to add *`Wire.setWireTimeout(3000, true);`* after *`Wire.setClock(400000);`* to prevent sketch from randomly freezing. *We also prepared a simplified sketch without unnecessary configurations.*

### Notes:
- *MPU-6050 is the cheapest among all. Usually cheapest sensors have significantly lower quality but not in this case thanks to DMP. During our classes don't buy the expensive MPU-6050 variants, the cheap one is enough.*
- *ICM-20948 is a successor to MPU-6050/MPU-9250 which has a magnetometer. Sparkfun has a library that leverages the same DMP algorithm for 9-dof. The problem is product availability, however there are concerns that Invensense doesn't use 9-dof for its DMP. We can't investigate it until we get one.*

## ISM330DHCX
### Calibration:
IMU example contains calibration on setup.

### Orientation:
Code returns both quaternions and euler angles, it's interrupt based with simple filtering so it works fine.

### Notes: 
- *ISM330DHCX is an industrial-grade version of LSM6DSOX with many enhancements. It's less noisy than MPU-6050.**
- *ISM330DHCX has built-in tilt direction, double tap, pedometer and free falling detection.*
- *It's compatible with Adafruit LSM6DS library. Unfortunately things like pedometer and double tap aren't implemented. I made my own implementation but it requires basic understanding of bare-metal programming.*
- *ISM330DHCX doesn't contain DMP but has Finite State Machine and Machine Learning Core for gesture recognition. ST provides iNEMO engine AHRS fusion algorithm but unfortunately it seems they removed it.* **Anyway, advanced features aren't possible to use on Arduino platform so it's not worth buying in our case.**
- *If IMU example doesn't work please check if your Arduino has different interrupt handling.*


## Pololu MinIMU-9 v5 9DOF
**You can use the adafruit library but the i2c address must be changed to 0x6b `lsm6ds33.begin_I2C(0x6b)` and `lis3mdl.begin_I2C(0x1E)`.**

The magnetometer is a separate sensor on this board.

### Calibration:
1. Magnetometer: MotionCal. Upload the calibration code and open the app.
2. Accelerometer and Gyroscope: calibrated via included code. AHRS example already has the gyro calibration included.

### Orientation:
There are libraries for these sensors from Pololu, Sparkfun and Adafruit. We will stick to Adafruit as we are using Adafruit AHRS library.

### Notes:
- *It's the same sensor you have on Adafruit Feather BLE.*
- *LIS3MDL magnetometer frequency is faster than magnetometer in LSM9DS1.*
- *You can use Adafruit library to use pedometer.*
- *Pololu libraries are very basic, there is an AHRS example but there are more steps to make it running https://github.com/pololu/minimu-9-ahrs-arduino*.
- *Need to investigate if magnetometer has flipped axes.*

## Arduino Nano 33 BLE (with LSM9DS1) DEPRECATED
**Why it's deprecated?**
1. The newer version of board has a different sensor but still no access to interrupt pin.
2. Without interrupts it's impossible to compute delta time along with Bluetooth LE communication. It still relies on polling method.
3. Let's be honest - the first time I got it, the library was unusable and my example usage is more like a failed attempt to make it work, eventually stopped wasting my time on this at some point.
4. In 2025 there are probably better alternatives. For example - equivalent board form adafruit includes Li-Po battery unit.

### Some info:
- The X and Y axes are inversed. (requires confirmation)
- The code is deprecated but the IMU example still works.
- Arduino equivalent of Adafruit AHRS libraries are confusing.

### What must be fixed:
- For some (probably stupid) reasons I can't set reversed unit conversion to fit motioncal requirements. Send a pull request if you can fix this.


## BNO055
### Calibration:
**Calibration is done internally on startup and adapts on runtime.** You can print calibration status, numbers between 0-3 are calibration grades.

### Orientation:
Reading quaternions is recommended by manufacturer. Later you can convert them to euler angles.

### Notes:
- For demanding projects, to be honest, if you can and your project can afford it, just buy BNO055 (this one is an old one so maybe newer/equivalent version). **The real-time calibration and build-in AHRS fusion algorithm saves a lot of headaches and time. But your project probably isn't THAT demanding.** *Quality of measurements especially matter on frequent orientation change. Based on my observation, eventually other sensors mentioned start losing it on rapid changes. However, those scenarios were extreme so there is no reason to buy it.*
- *This particular version doesn't have any double tap, gesture nor pedometer sensor.*
- It's update rate is up to 100Hz.
- **Computations are made internally so it doesn't waste Arduino resources.**