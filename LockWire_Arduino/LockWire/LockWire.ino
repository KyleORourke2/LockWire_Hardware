// LockWire.ino
// Written by: Kyle O'Rourke
// 1/19/2019
// CruzHacks UCSC

// Description:
// Electronic bike lock running on a DragonBoard 410c
// System uses an Accelerometer and GPS running on a
// Sparkfun 9-dof Razor IMU board. This software reads
// the incomming serial data from the Razor IMU board
// and determines if a theft has occured.

#include <SPI.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SFE_LSM9DS0.h>

static const int RXPin = 5, TXPin = 4;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// IMU DECLARATION //
#define LSM9DS0_XM  0x1D
#define LSM9DS0_G   0x6B
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

//IMU VALUES
float accel_normalized_avg;
unsigned long last_IMU_read_time;
float accelX, accelY, accelZ;
float gyro_angle_X = 0;
float gyro_angle_Y = 0;
float gyro_angle_Z = 0;
float last_x_angle = 0;
float last_y_angle = 0;
float last_z_angle = 0;

unsigned long old_time = 0;

void setup(){
  Serial.begin(4800);
  ss.begin(GPSBaud);

  uint16_t imustatus = dof.begin();
  if(imustatus == 18900){ //18900 = 0x49D4 in hex.

    //WRONG VALUES WILL BRICK SENSOR! SOURCE: THE HARD WAY.
    // Set data output ranges; choose lowest ranges for maximum resolution
    // Accelerometer scale can be: A_SCALE_2G, A_SCALE_4G, A_SCALE_6G, A_SCALE_8G, or A_SCALE_16G
      dof.setAccelScale(dof.A_SCALE_4G);
    // Gyro scale can be:  G_SCALE__245, G_SCALE__500, or G_SCALE__2000DPS
      dof.setGyroScale(dof.G_SCALE_2000DPS);
        //dof.setGyroScale(dof.G_SCALE_245DPS);
    // Magnetometer scale can be: M_SCALE_2GS, M_SCALE_4GS, M_SCALE_8GS, M_SCALE_12GS
        //dof.setMagScale(dof.M_SCALE_2GS);

    // Set output data rates:
    // Accelerometer output data rate (ODR) can be:
    //A_ODR_3125 (3.225 Hz), A_ODR_625 (6.25 Hz), A_ODR_125 (12.5 Hz), A_ODR_25, A_ODR_50,
    //A_ODR_100,  A_ODR_200, A_ODR_400, A_ODR_800, A_ODR_1600 (1600 Hz)
        dof.setAccelODR(dof.A_ODR_100); //Set accelerometer update rate at 100 Hz
    // Accelerometer anti-aliasing filter rate can be 50, 194, 362, or 763 Hz
    // Anti-aliasing acts like a low-pass filter allowing oversampling of accelerometer and rejection of high-frequency spurious noise.
    // Strategy here is to effectively oversample accelerometer at 100 Hz and use a 50 Hz anti-aliasing (low-pass) filter frequency
    // to get a smooth ~150 Hz filter update rate
        dof.setAccelABW(dof.A_ABW_50); //Choose lowest filter setting for low noise
    }
    else{
      Serial.println("IMU FAILED TO INITIALIZE.");
    }

  Serial.println("LockWire.ino");
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while(ss.available() > 0){
    if(gps.encode(ss.read())){
      displayInfo();
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  // IMU:
  get_accel(); // Get accel values.
  // Check if device has moved.
  
}

// Displays GPS Data:
void displayInfo(){
  //Serial.print(F("Loc: "));
  if(gps.location.isValid()){
    if((millis() - old_time) > 1000){
      old_time = millis();
      Serial.print(gps.location.lat(), 6);
      Serial.print(F("/"));
      Serial.print(gps.location.lng(), 6);
      Serial.println(F(";"));
    }
  }
  else if((millis() - old_time) > 1000){
    old_time = millis();
    Serial.println(F("INVALID"));
  }
}

//FINDS CURRENT ACCEL values.
void get_accel(){
  update_IMU_values(1,0,0);
  accelX = dof.calcAccel(dof.ax);
  accelY = dof.calcAccel(dof.ay);
  accelZ = dof.calcAccel(dof.az);
}

//Updates the ax, gx, mx variables to the most current ones avalible.
void update_IMU_values(int a, int g, int m){
  if(g == 1){
    dof.readGyro();
  }
  if(a == 1){
    dof.readAccel();
  }
  if(m == 1){
    dof.readMag();
  }
}
