#include <Wire.h> // For BMP280
#include <SPI.h>  // For BMP280
#include <Adafruit_BMP280.h> // For BMP280
#include <MPU6050_light.h>


#include <EEPROM.h>

/*
 * bmp280 INFO: A5 pin is SCL clock
 *              A4 pin is SDA
 * 
 */

Adafruit_BMP280 bmp; // I2C
MPU6050 mpu(Wire); // I2C
int sysState = 0;
unsigned long myTime = millis() ;
float launchThreshold = 1.05; // 1.1 g
float unPowThreshold = .20;

  // if z axis Accelleration >= Threshold true or false
  // if yes make sure count above threshold for >= .1 seconds
  
  // if z axis Accelleration >= 2 meters / second^2
  // if yes make sure count above threshold for >= .1 seconds
  // if yes MECO

  // detech apagey compare lengths of time 
  // if current altitude > (altitude - 1 sec) --> Ascending
  // if current altitude < (altitude - 1 sec) --> Decending
  

  // if we are =< 25 meters fire shoots and switch state

  // if we are less than 5m for more than 5 seconds landed

void navUpdate(){

  mpu.update();
  
}
void _getLocation(){
  
}

void _navUpdate(){
  mpu.update();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  unsigned bmpStatus = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);

  //BMP280 Settings
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);

  byte mpuStatus = mpu.begin();

  mpu.calcOffsets(true,true);
  Serial.println("Offsets done.");

  // Define what is needed for state transition to groud startup
}

void loop() {
  //timeKeeper();
  //stateIndication();
  //readBat();
  
  //navUpdate(); pulls navigation computer position velecovity altitude rates
  
  if (sysState == 0){ // Ground Startup
    //detectNavConverging(); way to get out of state
    navUpdate();
    long t1 = millis();
    float z1 = mpu.getAccZ();
    delay(100);
    navUpdate();
    long t2 = millis();
    float z2 = mpu.getAccZ();
    if (z1 >= launchThreshold && z2 >= launchThreshold){
        Serial.println(z1);
        Serial.println(z2);
        Serial.println("Blastoff");
        sysState = 1;
        Serial.println("System State Update: ");
      }
    }
 
    //TODO: Figure out why acceleration is dependant on angle
  if (sysState == 1){ // Launched powered flight Z pos active Y only vel
    delay(1000);
    //Serial.println(bmp.readAltitude(1013.25));
    long t1 = millis();
    float z1 = mpu.getGyroZ();
    //float x1 = mpu.getAccX();
    //float y1 = mpu.getAccY();
    //float a1 = (z1 + x1 ) /2;
    //Serial.println(a1);
    //Serial.println(mpu.getAccX());
    //Serial.println(mpu.getAccY());
    delay(100);
    navUpdate();
    long t2 = millis();
    float z2 = mpu.getGyroZ();
    Serial.println(z2);
    if (z1 <= unPowThreshold && z2 <= unPowThreshold){
        Serial.println(z1);
        Serial.println(z2);
        Serial.println("Apogee");
        sysState = 2;
        Serial.println("System State Update: ");
    }
  }

  if (sysState == 2){ // Unpowered flight
    
    
  }

  if (sysState == 3){ // ballistic decent

    
  }

  if (sysState == 4){ // chute decent

    
  }

  if (sysState == 5){ // landed decent

    
  }
}
