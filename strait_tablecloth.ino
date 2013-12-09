 
 /*
* Copyright (C) 2013 Anna Blumenkranz (info@annablumenkranz.de)*
This code is part of a bigger project investigating into communication exchange around the table 
and the social implications of dining culture in general. 
 */
 
 #include "pitches.h"
 

// set pins
int forkknifePin = 11;
int ledPin = A2;
int stretchPin = A3;
int forkknifeValue;
int stretchValue;
 
// notes in the melody:
int melody[] = {
  NOTE_DS1, NOTE_G2, NOTE_C6, NOTE_GS4, NOTE_DS4, 0, NOTE_D2, NOTE_E6};

  //NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };


void setup(){
   pinMode (ledPin, OUTPUT);
   pinMode (forkknifePin, INPUT);
   digitalWrite (forkknifePin, HIGH);
   pinMode (stretchPin, INPUT);
   digitalWrite (stretchPin, HIGH);
 }
 
 void loop(){
   forkknifeValue = digitalRead(forkknifePin);
   if (forkknifeValue == LOW){
     digitalWrite(ledPin, HIGH);
   }
   else{
     digitalWrite(ledPin, LOW);
   }
   
   stretchValue = analogRead(stretchPin);
   Serial.println(stretchValue);
   delay(1000);
   if(stretchValue < 30){
      //music: iterate over the notes of the melody
      for (int thisNote = 0; thisNote < 8; thisNote++) {
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000/noteDurations[thisNote];
        tone(9, melody[thisNote],noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        //noTone(9);
    }
   }  
 }
