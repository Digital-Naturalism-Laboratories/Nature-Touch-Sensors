# Six Channel ADC Touch Arduino Sketch

Here's some programs I made to super quickly make a touch-sensitive system. The example I have is for a set of touch sensitive frogs at a science display at a local animal rescue.

## How to Use
It's designed to be as simple and robust as possible. Just plug a single wire into each port on the Arduino A0-A5. These are now touch sensitive wires.  Plug the arduino into your laptop.

### Programs
You need to have Processing installed (the program is available for download free here https://processing.org/download ).
Then you can open up the processing program in the included folder and press the big play button. 
The program will start flashing colors when it is recieving data from the arduino. When the arduino detects a specific pin A0-A5 has been touched it will send a serial command to the processing program (0,1,2,3,4, or 5) that that specific wire has been touched.
The current processing program is set up to then play a sound (corresponding to the audio files labeled "0.wav" "1.wav" and so on.

### Calibration
They get calibrated automatically when you turn on, or reset, the arduino. If you plug a wire into digital pin 2 and another wire into GND, the system can also be re-calibrated whenever you touch 2-GND together. This makes for adding a simple calibration button.
If the system starts screwing up, simply just restart the processing program and the arduino. Make sure not to touch the sensors when the arduino is starting up.
