// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

#define dirPin1 6
#define stepPin1 7
#define motorInterfaceType 1

#define dirPin2 4
#define stepPin2 5

// Define some steppers and the pins the will use
//AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
//AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);

AccelStepper stepper1(AccelStepper::FULL2WIRE, stepPin1, dirPin1);
AccelStepper stepper2(AccelStepper::FULL2WIRE, stepPin2, dirPin2);

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(500.0);

  stepper2.setMaxSpeed(1000.0);
  stepper2.setAcceleration(500.0);

}

void loop() {
  if (stringComplete) {
    Serial1.println(inputString);
    runMotor(inputString.toInt());
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void runMotor(int position1) {
  Serial1.println(position1);
  stepper1.moveTo(position1);
  stepper2.moveTo(-position1);
  while (stepper1.distanceToGo() && stepper2.distanceToGo()) {
    stepper1.run();
    stepper2.run();
  }
}

void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '%') {
      stringComplete = true;
    }
  }
}
