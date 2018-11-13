// Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
/*
 * Requirements:
 * -voting/ballot box
 * -buttons
 * -two candidates
 * -save data
 * -elections should be fair!
 * -ballot
 */

/*
 * Changes:
 * [x]needs two buttons
 * [x]needs history, record votes & report numbers
 * []save data after press&release
 * []send current vote count
 */
/************************** Configuration ***********************************/
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin 5
#define BUTTON_PIN_1 5
#define BUTTON_PIN_2 4

// button state for button 1 & 2
bool current[] = {false, false};    //boolean arrays
bool last[] = {false, false};       //for two buttons!

int votes[] = {0,0};  //remembers votes cast by button 1 & 2

// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("digital");

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  io.run();

  // grab the current state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  if(digitalRead(BUTTON_PIN_1) == LOW)
    current[0] = true;
  else
    current[0] = false;

    if(digitalRead(BUTTON_PIN_2) == LOW)
    current[1] = true;
  else
    current[1] = false;

  // return if the value hasn't changed
  if(current[0] == last[0] && current[1] == last[1])
    return; //circuit breaker, go back to top of loop

//record vote here IF button has gone from pressed to released
if(current[0] != last[0] && current[0] == false){ //!current[0] is the same as "current[0] == false"
  votes[0] = votes[0] + 1;
  int voteCount = int(random(-1, 1));
  votes[0] = votes[0] + voteCount;
  if(voteCount == -1){
   votes[1] = votes[1] + 2;//if negative 1 must add two votes to keep vote count similar
   }
  if(voteCount == 0){
    votes[1] = votes[1] + 1;
  }
}
if(current[1] != last[1] && current[1] == false){ //!current[1] is the same as "current[1] == false"
  votes[1] = votes[1] + 1;
  
}

//assume a vote might be cast, send current results

  // save the current state to the 'digital' feed on adafruit io
  String message = String(votes[0]) +
                  ": " + 
                  String(votes[1]);

  Serial.print("sending votes -> ");
  Serial.println(message);
  //expect: "1, 2"
  digital->save(message);

  // store last button state
  last[0] = current[0];
  last[1] = current[1];

}
