#include <Arduino.h>
#include "ICM42605.h"

static const uint8_t CS_PIN = 5;
static const uint8_t INT_PIN = 38;

// an ICM42605 object with the ICM42605 sensor on VSPI bus 0
SPIClass vspi = SPIClass(VSPI);
ICM42605 imu(vspi, CS_PIN);

volatile bool dataReady = false;


void setImuFlag() {
  dataReady = true;
}

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU
  int status = imu.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }

  // attaching the interrupt to microcontroller pin INT_PIN
  pinMode(INT_PIN, INPUT);
  attachInterrupt(INT_PIN, setImuFlag, RISING);

  // set output data rate to 12.5 Hz
  imu.setAccelODR(ICM42605::odr12_5);
  imu.setGyroODR(ICM42605::odr12_5);

  // enabling the data ready interrupt
  imu.enableDataReadyInterrupt(); 

  Serial.println("ax,ay,az,gx,gy,gz,temp_C");
}

void loop() {
  if (!dataReady) return;

  dataReady = false;

  // read the sensor
  imu.getAGT();
  
  // display the data
  Serial.print(imu.accX(),6);
  Serial.print("\t");
  Serial.print(imu.accY(),6);
  Serial.print("\t");
  Serial.print(imu.accZ(),6);
  Serial.print("\t");
  Serial.print(imu.gyrX(),6);
  Serial.print("\t");
  Serial.print(imu.gyrY(),6);
  Serial.print("\t");
  Serial.print(imu.gyrZ(),6);
  Serial.print("\t");
  Serial.println(imu.temp(),6);
}
