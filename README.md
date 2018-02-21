# position_estimator-FRDM-KL25Z4
A position estimator for my Cambridge University 4B25 coursework submission. The program uses the built in Accelerometer on the Freedman Kl25z board and an external Adafruit Ultimate GPS module to track the position and velocity of the unit.

## Structure

The code running on the system is found in the folder Sources/
Code for testing the estimator is found in the folder Testing/
The Linear Algebra Package for C++ Eigen is a requirement and is found in the folder External/
Hardware specific files for the KL25z is found in the generated source folder Project_settings/

### Sources
Contains a main.c function that includes the program flow 

#### Accelerometer
Contains the api accelerometer.h and corresponding source code for accessing the the data from the acceleromter.
It also contains the Kinetis accelerometer files. 

#### Board
Contains hardware interface defines, code and pin initialization 

#### Common
Contains the file types.h defining the interfacing structs and some useful code

#### Display
Contains the API file for using and writing on the OLED display. 
It contains an implementation of a hardware interface for  (Adafruit_SSD1331) a external graphix library (Adafruit_GFX + fonts).
It also contains low level functions for interfacing over SPI with the device. 

#### Positioning
Contains the API for accessing the GPS data (gps.h/c). 
It also contains a driver for interupt based UART for communication (gps_serial.c/h)
Lastly it contains a class for parsing and accesing NMEA messages. 

#### Utilities
This folder includes required interrupt drivers and debugging interface. 

