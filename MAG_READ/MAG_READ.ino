 // These two includes are necessary to read
// the LIS3MDL (magnetometer)
# include <Wire.h>
# include <LIS3MDL.h>

// LIS3MDL is a class.  We create an instance
// of this class called "mag".  We will then
// use mag to access the device by method
// functions and variables inside the class.
LIS3MDL mag;

// Variables for Magnetometer reading

float x;
float x_c;
float y;
float y_c; 
float z;
float z_c;

// From XL

float range_x = 3739.0;
float range_y = 2770.0;
float range_z = 656.0;

float offset_x = range_x / 2;
float offset_y = range_y / 2;
float offset_z = range_z / 2;

float avg_range = (range_x + range_y + range_z) / 3;

float scale_x = avg_range / range_x;
float scale_y = avg_range / range_y;
float scale_z = avg_range / range_z;

// Sensitivity settings from datasheet: https://www.st.com/resource/en/datasheet/lis3mdl.pdf
// + - 4 Gauss - 6842
// + - 8 Gauss - 3421
// + - 12 Gauss - 2281
// + - 16 Gauss - 1711
// The sheet equation says to multiply by sensitivity, but the unit it produces is huge

float sensitivity = 6842.0;

// MAG_TEST //

void magRead() {
  mag.read();

  x = mag.m.x;
  y = mag.m.y;
  z = mag.m.z;

  x_c = ( scale_x * (x - offset_x) ) / sensitivity;
  y_c = ( scale_y * (y - offset_y) ) / sensitivity;
  z_c = ( scale_z * (z - offset_z) ) / sensitivity;
}

void magPrint() {
 
  Serial.print(x_c);
  Serial.print(",");
  Serial.print(y_c);
  Serial.print(",");
  Serial.println(z_c);
  
}

// 

void setup() {

  // Start the wire library for i2c.
  // Note: do not add this command into
  // a class constructor. It must occur
  // (or be called) from setup().
  Wire.begin();

  // Serial for debug output
  Serial.begin(9600);
  Serial.println("***RESET***");
  delay(1000);

  // Check we have intialised commmuncation
  if (!mag.init() ) {  // no..? :(

    // Since we failed to communicate with the
    // magnetometer, we put the robot into an infinite
    // while loop and report the error.
    while(1) {
      Serial.println("Failed to detect and initialize magnetometer!");
      delay(1000);
    }
  }

  // Initialisation was ok.
  // Set to default settings.
  mag.enableDefault();

  Serial.begin(9600);
  delay(1500);

  // Seems pointless to have this in the magPrint loop as it clutters the results
  // It's also pointless to have it here once its been left running longer than a few seconds
  
  Serial.print("X:");
  Serial.print(",");
  Serial.print("Y:");
  Serial.print(",");
  Serial.println("Z:");
  
} // end of setup()

void loop() {

  magRead();

  magPrint();
  
  // Short delay to keep things
  // slow enough to observe.
  // There is a limit to how fast you
  // can make i2c readings.
  delay(100);

} // end of loop()
