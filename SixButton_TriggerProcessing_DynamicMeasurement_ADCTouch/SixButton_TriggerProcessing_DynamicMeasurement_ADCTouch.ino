/* SixButton_TriggerProcessing_AnalogOut_DynamicMeasurement_ADCTouch

Created a Generic 6 channel Sensor array that can send data via serial to something like Processing to trigger stuff.

(or all 6 sensors can just control the PWM of 6 different things)



    This is one of the easiest ways to make something reliably touch sensitive!

    Plug an output into Pin 11, and it will change in response to the ADC touch sensor in pin A0

    pin 11 is chosen because it is the right spacing on an Uno for a piezo buzzer to reach the gnd pin!)

   Remember ADC touch works better when not plugged into wall power, but run off a battery!

   This extends Martin2250's ADC touch examples
   to have the library built in, for rapid deployment in workshop settings


       --THE KEY DIFFERENCE BETWEEN THIS AND THE STATIC EXAMPLE - is that this does not stay calibrated to a single value,
       instead it takes a constant running average and outputs any recent CHANGES the touch sensor notices

       so it can adapt to lots of changes int he situation, but also it won't give the same results always, or when touched for an extended time


*/

#include "ADCTouch.h"

/*Smoothing Stuff*/
//This is the code to compute the running average / smoothing library
#include "Ewma.h"

Ewma adcFilter[6](0.01 );   // value here says how much smoothing to do, which changes the response rate,  .01 == lots of smoothing  .1= Less smoothing - faster to detect changes, but more prone to noise
float filteredVal[] = {0, 0, 0, 0, 0, 0};

/*Touch Stuff*/
int touchPins[] = {A0, A1, A2, A3, A4, A5}; // This pin is what you connect your touch sensor to, It's literally just a wire! so easy!
int pincount = 6;

int value[] = {0, 0, 0, 0, 0, 0};


//These two values map the touch sensor's value. Tweak these if you aren't getting the response you want. Look at the Serial Plotter to get a feel for the min and max values you are getting!
int valhigh[] = {150, 150, 150, 150, 150, 150};
int vallow[] = {20, 20, 20, 20, 20, 20}; // making this one a little above zero to kill the noise
int diff[] = {0, 0, 0, 0, 0, 0}; // This is the value that will be mapped to our output

int valconstrained[] = {0, 0, 0, 0, 0, 0}; // This is the value that will be mapped to our output

int ref[6];     //This is the value we use for calibrating, we store this value and remove it from the raw data as an offset
int triggerThresh[] = {20, 20, 20, 20, 20, 20}; // The value to trigger that the input is being touched (and likely send something to processing)

//Onboard Outputs
//I have included an additional Output pin if you want something triggered just on and off when the touch goes past a certain threshold
int digitalPinOut = 13;
int digitalPinThresh = 20; //you can tweak this threshold to change when the pinout is triggered

// if you want to be able to recalibrate the Capactive Touch sensor, you can plug  a wire (or button) from pin 2 to GND
int recalibratePin = 2;

int analogPinOut[] = {11, 10, 9, 6, 5, 3}; // This pin is mapped to the touch sensitive pin // This is all the standard PWM pins on  an Uno
// connect your LED, small motor, vibration motor, piezo buzzer or whatever you want to activate when something is touching the touch pin



bool debugSerialmode = false;


void setup() {
  Serial.begin(9600);

  //Setup all the sensors
  for (int p = 0; p < pincount; p++) {

    ref[p] = ADCTouch.read(touchPins[p], 500);    //Calibrate the capactive touch sensor to whatever the thing was touching when it first starts

    digitalWrite(analogPinOut[p], HIGH); // give us a twinkle when we start to let us know we are on and done calibrating
    delay(70);
    digitalWrite(analogPinOut[p], LOW); // give us a twinkle when we start to let us know we are on and done calibrating

  }

  //Setup the other pins
  pinMode(analogPinOut, OUTPUT);
  pinMode(digitalPinOut, OUTPUT);
  pinMode(recalibratePin, INPUT_PULLUP);

}

void loop()
{

  //Loop through all analog pins
  for (int p = 0; p < pincount; p++) {

    //Read all the sensors
    value[p] = ADCTouch.read(touchPins[p]);   //no second parameter




    //If the recalibrate pin is triggered, recalibrate the touch sensor
    if (0 == digitalRead(recalibratePin)) {
      ref[p] = ADCTouch.read(touchPins[p], 500);   // Calibrate the capactive touch sensor to whatever the thing is now touching
      adcFilter[p].reset();
      filteredVal[p] = 0; //reset the smoothed value too
    }





    value[p] -= ref[p];       //remove offset from the calibration

    filteredVal[p] = adcFilter[p].filter(value[p]);

    //Subtract the raw from the average to see how much difference there is!

    diff[p] = abs(filteredVal[p] - value[p]);

    // Map the values to output values
    valconstrained[p] = map(diff[p], vallow[p], valhigh[p], 0, 255); // map from 0 to 255 for output for an LED or Buzzer
    valconstrained[p] = constrain(valconstrained[p], 0, 255); // extra make sure it doesn't go past limits of o or 255


    //Serial Prints for Debugging visualization
    if (debugSerialmode == true) {
     
      /*
      Serial.print(p);
      Serial.print("_RawValue:");
      Serial.print(value[p]);  //send Raw calibrated reading for debugging
      Serial.print(","); // let us print multiple values
      */


      /*
        Serial.print("RawValue:");
        Serial.print(value0);  //send Raw calibrated reading for debugging
        Serial.print(","); // let us print multiple values


        Serial.print("AvgValue:");
        Serial.print(filteredVal0);  //send Raw calibrated reading for debugging
        Serial.print(","); // let us print multiple values
      */



      //Now instead of just taking these raw values, what we are really interested in is the how different the average is from the current state
      //if it is different, something happened!
      Serial.print(p);
      Serial.print("_Difference:");
      Serial.print(diff[p]);  //Diff
      Serial.print(","); // let us print multiple values

    }


    //Trigger the analog out
    analogWrite(analogPinOut[p], diff[p]);

    //Trigger the digital Output if past threshold (optional)
    if (valconstrained[p] > triggerThresh[p]) {
      if (!debugSerialmode) {

        Serial.write(p);               // send number of pin touched to Processing to queue sample 0
        delay(1500);
      }
    }
    else {
      if (!debugSerialmode) {
        Serial.write(255);               // send X to Processing // Nothing happening
      }
    }

    delay(1); // tiny delay to smooth out things
  }


if(debugSerialmode){
  Serial.println();
}
}
