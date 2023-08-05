/**
 *   Take in Arduino inputs and trigger sounds in processing
 
 NOTE - You may need to change the serial number to connect to your arduino!
 
 based off
 Processing Sound Library, Example 4
 *
 * This example shows how to make a simple keyboard-triggered sampler with the Sound
 * library. In this sketch 5 different short samples are loaded and played back at
 * different speeds, which also changes their perceived pitch by one or two octaves.
 *
 * Load this example with included sound files from the Processing Editor:
 * Examples > Libraries > Sound > Soundfile > Keyboard
 */

import processing.serial.*;


Serial myPort;  // Create object from Serial class
int val;     // Data received from the serial port



import processing.sound.*;

SoundFile[] file;

// Define the number of samples
int numsounds = 5;

// Define a variable to store the randomly generated background color in
int backgroundColor[] = {255, 255, 255};



PFont f;


void setup() {

 if(Serial.list().length>0){ 
  printArray(Serial.list()); //This lists all the available serial ports, you may need to choose a different one
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  
  myPort = new Serial(this, portName, 9600);
  myPort.clear();
     // Throw out the first reading, in case we started reading 
  // in the middle of a string from the sender.
  val = myPort.read();
  val = 255;
 }
 else{
     println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

  println("You need to connect an arduino to the USB");
       println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

  exit();
 }


  size(640, 360);


  // Create a Sound renderer and an array of empty soundfiles
  file = new SoundFile[numsounds];

  // Load 5 soundfiles from a folder in a for loop. By naming
  // the files 1.aif, 2.aif, 3.aif, ..., n.aif it is easy to iterate
  // through the folder and load all files in one line of code.
  for (int i = 0; i < numsounds; i++) {
    file[i] = new SoundFile(this, (i) + ".wav");
  }



  //printArray(PFont.list());
  f = createFont("JosefinSans-Bold.ttf", 24);
  textFont(f);
  textAlign(CENTER, CENTER);
}

void draw() {
  background(backgroundColor[0], backgroundColor[1], backgroundColor[2]);

  if ( myPort.available() > 0) {  // If data is available,
    for (int i = 0; i < 3; i++) {
      backgroundColor[i] = int(random(255));
    }
    val = myPort.read();         // read it and store it in val
    println(val); //print it out in the console
    textSize(206);
    text(val, width/2, height/2);
  }

  frogTouched();
}

void frogTouched() {

  /*
  play parameters
   
   rate(float)relative playback rate to use. 1 is the original speed. 0.5 is half speed and one octave down. 2 is double the speed and one octave up.
   pos(float)the panoramic position of this sound unit from -1.0 (left) to 1.0 (right). Only works for mono audiosamples!
   amp(float)the desired playback amplitude of the audiosample as a value from 0.0 (complete silence) to 1.0 (full volume)
   add(float)offset the output of the generator by the given value
   cue(float)position in the audiosample that playback should start from, in seconds.
   
   */

  // We use a boolean helper variable to determine whether one of the branches
  // of the switch-statement was activated or not
  boolean validKey = true;

  switch(val) {
  case 0:
    //file[0].play(1.0, 0.0, 1.0, 0.0, 2.0);
    //file[0].jump(5.0); //skip ahead past the narrator voice
        file[0].playFor(2.0,5.0); //skip ahead past the narrator voice

    delay(1000);
    break;

  case 1:
    file[1].playFor(2.0,5.0); //skip ahead past the narrator voice
    delay(1000);
    break;

  case 2:
        file[2].playFor(2.0,5.0); //skip ahead past the narrator voice
  delay(1000);
    break;

  case 3:
    file[3].playFor(2.0,5.0); //skip ahead past the narrator voice
    delay(1000);
    break;

  case 4:
    file[4].playFor(2.0,5.0); //skip ahead past the narrator voice
    delay(1000);
    break;

  case 5:
    file[5].playFor(2.0,5.0); //skip ahead past the narrator voice
    delay(1000);
    break;

  case 255:
  break;


    // no valid key was pressed, store that information
  default:
    validKey = false;
  }

  /*
  // If a new sample playback was triggered, change the background color
  if (validKey) {
    for (int i = 0; i < 3; i++) {
      backgroundColor[i] = int(random(255));
    }
  }
  */
}
