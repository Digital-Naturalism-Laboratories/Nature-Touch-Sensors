
/* OneButton_SERVOOut_StaticMeasurement_ADCTouch
    This is one of the easiest ways to make something reliably touch sensitive!
    Plug a SERVO into Pin 11, and it will change in response to the ADC touch sensor in pin A0

    pin 11 is chosen because it is the right spacing on an Uno for a piezo buzzer to reach the gnd pin!)

   Remember ADC touch works better when not plugged into wall power, but run off a battery!

   This extends Martin2250's ADC touch examples
   to have the library built in, for rapid deployment in workshop settings

*/



#include "ADCTouch.h"
#include <Servo.h>
Servo myservo;  // create servo object to control a servo


int touchPin = A0; // This pin is what you connect your touch sensor to, It's literally just a wire! so easy!

int servoPinOut = 11; // This pin is mapped to the touch sensitive pin
//Connect your servo to this

//These two values map the touch sensor's value. Tweak these if you aren't getting the response you want. Look at the Serial Plotter to get a feel for the min and max values you are getting!
int val0high = 120;
int val0low = 0;

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
  myservo.attach(servoPinOut);  // attaches the servo to the software servo object

  //Setup the pins
  pinMode(servoPinOut, OUTPUT);
  pinMode(digitalPinOut, OUTPUT);
  pinMode(recalibratePin, INPUT_PULLUP);


  ref0 = ADCTouch.read(touchPin, 500);    //Calibrate the capactive touch sensor to whatever the thing was touching when it first starts

  myservo.write(180); // give us a twinkle when we start to let us know we are on and done calibrating
  delay(50);
}

void loop()
{
  int value0 = ADCTouch.read(touchPin);   //no second parameter

  //If the recalibrate pin is triggered, recalibrate the touch sensor
  if (0 == digitalRead(recalibratePin)) {
    ref0 = ADCTouch.read(touchPin, 500);   // Calibrate the capactive touch sensor to whatever the thing is now touching
  }

  value0 -= ref0;       //remove offset from the calibration

  Serial.println(value0);  //send Raw calibrated reading for debugging
  delay(5); // tiny delay to smooth out things


  // Map the values to output values
  val0constrained = map(value0, val0low, val0high, 0, 255); // map from 0 to 255 for output for an LED or Buzzer
  val0constrained = constrain(val0constrained, 0, 255); // extra make sure it doesn't go past

  //Trigger the Servo
  myservo.write(val0constrained);


  //Trigger the digital Output if past threshold (optional)
  if (val0constrained > digitalPinThresh) {
    digitalWrite(digitalPinOut, HIGH);
  }
  else {
    digitalWrite(digitalPinOut, LOW);

  }


}
