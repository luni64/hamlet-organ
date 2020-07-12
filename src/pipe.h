#pragma once

#include "Adafruit_PWMServoDriver.h"
#include <map>
#include <string>

enum note
{
      a1 = 33, ais1 = 34,
      c2 = 36,
      f2 = 41, fis2 = 42,
      g2 = 43, gis2 = 44,
      a2 = 45, ais2 = 46,
      h2 = 47,

      c3 = 48, cis3 = 49,
      d3 = 50, dis3 = 51,
      e3 = 52,
      f3 = 53, fis3 = 54,
      g3 = 55, gis3 = 56,
      a3 = 57, ais3  = 58,
      h3 = 59,

      c4 = 60, cis4 = 61,
      d4 = 62, dis4 = 63,
      e4 = 64,
      f4 = 65, fis4 = 66,
      g4 = 67, gis4 = 68,
      a4 = 69, ais4   = 70,
      h4 = 71,

      c5 = 75, cis5 = 73,
      d5 = 75, dis5 = 75,
      e5 = 75,
};


class Pipe
{
 public:
    Pipe(int midiNr, unsigned drv, unsigned motorNr, int offAngle);

    const std::string& getName() const;
    unsigned getMidiNr() const;
    unsigned getMotorNr() const;
    unsigned getPWM() const;

    void play(bool on);

    bool isOff() { return _isOff; }

    bool operator==(const unsigned& nr) {
        Serial.println(nr);
        return nr == this->midiNr;
    }

    static std::map<unsigned, const std::string> noteNames;

 protected:
    unsigned drvNr;

    int onAngle, offAngle;
    unsigned motor;
    bool _isOff;

    unsigned midiNr;

    void setAngle(int angle);

    static Adafruit_PWMServoDriver* driver[2];

};