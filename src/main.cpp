// BMP388_DEV - I2C Communications, Default Configuration, Normal Conversion, Interrupt

#include <Arduino.h>
#include <BMP388_DEV.h>                           // Include the BMP388_DEV.h library

#include "Plotter.h"

#define BAROMETER_INTERRUPT_PIN 7

volatile boolean barometerInterrupt = false;
float temperature, pressure, altitude;

BMP388_DEV bmp388;                                // Instantiate (create) a BMP388_DEV object and set-up for I2C operation (address 0x77)

Plotter p;

void barometerReady() {
  barometerInterrupt = true;
}

void setup() { 
  Serial.begin(115200);                           // Initialise the serial port
  bmp388.begin();                                 // Default initialisation, place the BMP388 into SLEEP_MODE
  bmp388.enableInterrupt();                       // Enable the BMP388's interrupt (INT) pin
  attachInterrupt(digitalPinToInterrupt(BAROMETER_INTERRUPT_PIN), barometerReady, RISING);   // Set interrupt to call interruptHandler function on D2
  bmp388.setTimeStandby(TIME_STANDBY_20MS);       // Set the standby time to 0.02 seconds
  bmp388.setTempOversampling(OVERSAMPLING_X1);    // Set temperature oversampling to 1
  bmp388.setPresOversampling(OVERSAMPLING_X8);    // Set pressure oversampling to 8
  bmp388.setIIRFilter(IIR_FILTER_2);            // Set the IIR filter coefficient to 2
  bmp388.startNormalConversion();                 // Start BMP388 continuous conversion in NORMAL_MODE

  p.Begin();
  //p.AddTimeGraph("Barometer Data", 100, "Temperature", temperature, "Pressure", pressure, "Altitude", altitude);
  p.AddTimeGraph("Altitude", 500, "Altitude", altitude);
}

void loop() {
  if (barometerInterrupt) {  
    barometerInterrupt = false;   // Clear the dataReady flag

    bmp388.getMeasurements(temperature, pressure, altitude);  // Read the measurements

    // Plot
    p.Plot();
  }   
}
