#pragma once

#include "Adafruit_PWMServoDriver.h"
#include "pipe.h"
#include <vector>

class Organ
{
 public:
    Organ(Stream* debugStream);
    void begin();
    void playNote(unsigned midiNr, bool on);
    void allOn();
    void allOff();

    const std::vector<Pipe*>& getAllPipes() const { return pipes; }

 protected:
    Adafruit_PWMServoDriver pwm0, pwm1;

    std::vector<Pipe*> pipes;
    Pipe& motorFromNote(unsigned note);

    Stream* debugStream;
};