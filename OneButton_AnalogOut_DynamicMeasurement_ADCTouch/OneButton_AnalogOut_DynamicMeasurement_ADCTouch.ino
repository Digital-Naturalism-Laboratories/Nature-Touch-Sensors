/* OneButton_AnalogOut_DynamicMeasurement_ADCTouch


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

//This is the code to compute the running average / smoothing library
#include "Ewma.h"

Ewma adcFilter1(0.01 );   // value here says how much smoothing to do, which changes the response rate,  .01 == lots of smoothing  .1= Less smoothing - faster to detect changes, but more prone to noise
float filteredVal0 = 0;


int touchPin = A0; // This pin is what you connect your touch sensor to, It's literally just a wire! so easy!

int analogPinOut = 11; // This pin is mapped to the touch sensitive pin
// connect your LED, small motor, vibration motor, piezo buzzer or whatever you want to activate when something is touching the touch pin


//These two values map the touch sensor's value. Tweak these if you aren't getting the response you want. Look at the Serial Plotter to get a feel for the min and max values you are getting!
int val0high = 150;
int val0low = 20; // making this one a little above zero to kill the noise

int val0constrained = 0; // This is the value that will be mapped to our output

int ref0;     //This is the value we use for calibrating, we store this value and remove it from the raw data as an offset

//I have included an additional Output pin if you want something triggered just on and off when the touch goes past a certain threshold
int digitalPinOut = 12;
int digitalPinThresh = 30; //you can tweak this threshold to change when the pinout is triggered


// if you want to be able to recalibrate the Capactive Touch sensor, you can plug  a wire (or button) from pin 2 to GND
int recalibratePin = 2;


void setup()
{
  Serial.begin(9600);

  //Setup the pins
  pinMode(analogPinOut, OUTPUT);
  pinMode(digitalPinOut, OUTPUT);
  pinMode(recalibratePin, INPUT_PULLUP);


  ref0 = ADCTouch.read(touchPin, 500);    //Calibrate the capactive touch sensor to whatever the thing was touching when it first starts

  digitalWrite(analogPinOut, HIGH); // give us a twinkle when we start to let us know we are on and done calibrating
  delay(50);
}

void loop()
{
  int value0 = ADCTouch.read(touchPin);   //no second parameter

  //If the recalibrate pin is triggered, recalibrate the touch sensor
  if (0 == digitalRead(recalibratePin)) {
    ref0 = ADCTouch.read(touchPin, 500);   // Calibrate the capactive touch sensor to whatever the thing is now touching
    adcFilter1.reset();
    filteredVal0 = 0; //reset the smoothed value too
  }

  value0 -= ref0;       //remove offset from the calibration

  filteredVal0 = adcFilter1.filter(value0);

  Serial.print("RawValue:");
  Serial.print(value0);  //send Raw calibrated reading for debugging
  Serial.print(","); // let us print multiple values


  Serial.print("AvgValue:");
  Serial.print(filteredVal0);  //send Raw calibrated reading for debugging
  Serial.print(","); // let us print multiple values



  //Subtract the raw from the average to see how much difference there is!

  //Now instead of just taking these raw values, what we are really interested in is the how different the average is from the current state
  //if it is different, something happened!
  Serial.print("Difference:");
  int diff = abs(filteredVal0 - value0);
  Serial.println(diff);  //Diff

  // Map the values to output values
  val0constrained = map(diff, val0low, val0high, 0, 255); // map from 0 to 255 for output for an LED or Buzzer
  val0constrained = constrain(val0constrained, 0, 255); // extra make sure it doesn't go past

  //Trigger the analog out
  analogWrite(analogPinOut, val0constrained);


  //Trigger the digital Output if past threshold (optional)
  if (val0constrained > digitalPinThresh) {
    digitalWrite(digitalPinOut, HIGH);
  }
  else {
    digitalWrite(digitalPinOut, LOW);
  }

  delay(5); // tiny delay to smooth out things

}
