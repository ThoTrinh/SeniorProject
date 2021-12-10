/*********************
Name:           Engine-Monitoring-Project
Created:        12/09/2021
Authors:        Tho Trinh, Robert, Pete, Nhi, Triston

Description:    This measures the torque using strain gauge readings on the dynometer. It should be self calibrating
**********************/

// Including Necessary Libraries
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <HX711_ADC.h>
#include <math.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//I2C Defined Pins:
const int HX711_dout = 20; //mcu > HX711 dout pin
const int HX711_sck = 21; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

int initial_torque = 0;
unsigned long t = 0;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  //refresh the display
  lcd.clear();

  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  LoadCell.setCalFactor(1);
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);

  // Check if I2C wires are connected to pin 20 and 21
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }

  // Wait 3 seconds for engine to start up before calibration
  delay(3000);

  boolean readyForCalibration = true;

  // print out "Calibrating" on to LCD
  lcd.setCursor(0,1);
  lcd.print("Calibrating...");
  
  // get smoothed value from the dataset:
  for(int i = 0; i < 100; ++i)
  {
    while(readyForCalibration != true) { // make sure that new Data is ready before we add another data point
        if (LoadCell.update()) {
          readyForCalibration = true;
        }
      }

    // when new data ready, add another data point to average
    if (readyForCalibration) {
      initial_torque += LoadCell.getData();
    }
    
    readyForCalibration = 0;
  }

  // take average of starting value (when engine off), then we will use this to calibrate our torque measurements
  initial_torque = initial_torque / 100;

  // This removes the last 3 digits from our reading to get rid of electrical noise ( ex. 1459821 -> 1459 )
  initial_torque = initial_torque / 1000;

  lcd.clear();  // Clear the screen of the calibrating message
  // Print on the first row that we will output in Torque (in-lb)
  lcd.setCursor(0,0);
  lcd.print("Torque (in-lb)");
}

void loop() {
  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; // increase value to slow down print activity 

  // check for new data/start next conversion:
  if (LoadCell.update()) {
    newDataReady = true;
  }

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      // get value from strain gauges
      float strain_gauge_value = LoadCell.getData();
      
      // remove last three digits from strain gauge reading to remove electrical noise
      int rounded_strain_gauge_value = rounded_strain_gauge_value/1000;
      
      int torque = rounded_strain_gauge_value - initial_torque; // initial_torque was the calibration value when engine was off.
                                                                // all torque outputs will be based off of the initial value when engine's off
                                                                // torque is the calibrated strain gauge value
                                                                
      // this accounts for some of the movement in strain gauge reading when engine is off
      if (torque < 6){
        torque = 0;
      }
      
      int real_torque = 0; // real_torque is the torque that we will output to the lcd screen
      if (torque < -10){ // if strain gauge reading is this low, we have to recalibrate system
        
        lcd.setCursor(0,0);
        lcd.print("RECALIBRATE");
        lcd.setCursor(0,1);
        lcd.print("  SYSTEM");
        
      } else {  // if strain gauge reading is okay, we calculate torque from strain gauge reading
        
        if (torque <= 21) { // if strain gauge reading is low, we use this equation 
          real_torque = .8 * exp(.25 * torque); 
        } else { // if strain gauge reading is high, we use this equation
          real_torque = ( .9 * exp( .153 * torque) ) - ( .3 * exp( .09 * torque));
        }

        //Start on the 2nd row to print our torque value on second row
        lcd.setCursor(0,1);
        lcd.print(real_torque);
      }
      
      newDataReady = 0; // reset newDataReady
      t = millis(); // update t
    }
  }  
}
