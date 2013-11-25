/*
 *    Copyright (C) 2013 Gareth Foote (gareth.foote@gmail.com)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Affero General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <Servo.h> 

#define SERVO_INACTIVE_POS 90
#define SERVO_LEFT_POS 135
#define SERVO_RIGHT_POS 45

// Set pins.
int pirPinLeft = 2;
int pirPinRight = 3;
int servoPin = 9;
int micPin = A0;
int ledPin = 13;

// PIR - motion detection.
int pirInactiveDuration = 5000;
unsigned long pirPauseTimer;

// Servo.
Servo servo;
int servoTargetPos;
int servoPos = SERVO_INACTIVE_POS;
int servoPosVariation = 45;
int servoRoam = 0;
int servoMoveSpeed = 4;

// Microphone sampling. (Currently unused)
int maxValue;
int minValue;
int micSampleDuration = 500; // Amount in milliseconds to sample data
int volume; // this roughly goes from 0 to 700
unsigned long timerMic;

boolean initialise = false;
int increment = 0;

void setup(){
 
  Serial.begin(9600); 
  pinMode(pirPinLeft, INPUT);
  pinMode(pirPinRight, INPUT);

  servo.attach(servoPin);
  servo.write( SERVO_INACTIVE_POS );
  // resetMicValues();
  pirPauseTimer = millis();

}

void loop(){
  
  // Pause for a few seconds to let PIR 
  // sensors initialise.
  if( initialise == false ){
    delay(5000);
    initialise = true;
    return;
  }

  // If we are not paused here then check PIR sensors.
  if( millis() - pirPauseTimer > pirInactiveDuration ){
    
    // LED indicates PIR sensors are active.
    digitalWrite( ledPin, HIGH );
    
    boolean pirLeft = checkPIR(pirPinLeft);
    boolean pirRight = checkPIR(pirPinRight);
    
    if( pirLeft == true ){
      Serial.println("++++++Left");
      // Set servo target position to where sensor was activated.
      servoTargetPos = SERVO_LEFT_POS; // +random(0-servoPosVariation, servoPosVariation);
    }
    
    if( pirRight == true ){
      Serial.println("---Right");
      // Set servo target position to where sensor was activated.
      servoTargetPos = SERVO_RIGHT_POS; // +random(0-servoPosVariation, servoPosVariation);
    }
    
    // If either PIR sesnors are active then
    // set a pause time and 
    if( pirLeft == true || pirRight == true ){
      pirPauseTimer = millis();
      servoRoam = 0;
      // Servo position increment may be positive or negative 
      // depending on direction.
      increment = ( servoPos < servoTargetPos) ? servoMoveSpeed : 0-servoMoveSpeed;
     }
 
  } else {
     
    // ...or else if we are paused here then
    // sensors have detected movement and 
    // we can move the servo and set LED off.
    digitalWrite( ledPin, LOW );
    moveServo();

  }
  
}

// Checks if the PIR sensor at a particular pin is active.
boolean checkPIR(int pirPin){
  
  int pirVal = digitalRead(pirPin);
  return pirVal == HIGH;
  
}

void moveServo(){

  // Block servo movement 1 second before PIRs start.
  // Writing to the servo seems to interfer with the PIRs.
  if( millis() - pirPauseTimer > pirInactiveDuration-1000 ){
    // Serial.println("BLOCKED"); 
    return;
  }
  
  // If incremenet is positive and servo position is less than (<) target
  // OR if increment is negative and servo position is greater than(>) target.
  if(( increment > 0 && servoPos < servoTargetPos+servoRoam )
      || ( increment < 0 && servoPos > servoTargetPos+servoRoam )){
    // ...then write a new position to servo
    servo.write(servoPos);
    // and increment position (creates slower movement).
    servoPos += increment; 
    delay(15);
  } else {
    // Once target position has been reached then add some random 
    // amount to {servoRoam} to make it continue moving a bit.
    servoRoam = random(0-servoPosVariation, servoPosVariation);
    increment = ( servoPos < servoTargetPos+servoRoam) ? servoMoveSpeed/2 : 0-(servoMoveSpeed/2);
    delay(100);
  }
}

void checkVolume(){
 
    int soundLevel = analogRead(micPin);
    int currentValue=abs(soundLevel-600);
  
    if (currentValue < minValue) {
        minValue = currentValue;
    } 
    if (currentValue > maxValue) {
        maxValue = currentValue;
    }

    if (millis() - timerMic >= micSampleDuration) {
        volume = maxValue - minValue;
        resetMicValues();
    }
    
}

void resetMicValues(){
  
    maxValue = 0;
    minValue = 1024;
    timerMic = millis(); 

}

