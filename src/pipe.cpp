#include "pipe.h"

const std::string& Pipe::getName() const
{
    return noteNames[midiNr];
}

unsigned Pipe::getMidiNr() const
{
    return midiNr;
}

unsigned Pipe::getMotorNr() const
{
    return motor;
}

unsigned Pipe::getPWM() const
{
    return drvNr;
}

Pipe::Pipe(int midiNr, unsigned drvNr, unsigned motor, int offAngle)
{
    this->drvNr = drvNr;
    this->motor = motor;
    this->offAngle = offAngle;
    this->onAngle = offAngle + 15;
    this->midiNr = midiNr;
}


void Pipe::play(bool noteOn)
{
    if (noteOn)
    {
        setAngle(onAngle);
        _isOff = false;
        Serial.printf("ON:  %d (%s) M:%d-%d\n", midiNr, getName().c_str(),  drvNr, motor);
    } else
    {
        setAngle(offAngle);
        _isOff = true;
        Serial.printf("OFF: %d (%s) M:%d-%d\n", midiNr, getName().c_str(),  drvNr, motor);
    }
}

void Pipe::setAngle(int angle)
{
    constexpr unsigned pwmFreq = 50.0;
    constexpr double pwmPeriod = 1000.0 / pwmFreq;
    constexpr double pwmFac = 4096 / pwmPeriod;
    constexpr double pwm0deg = pwmFac * 1;   // 1ms
    constexpr double pwm180deg = pwmFac * 2; // 2ms
    constexpr float pwmSlope = (pwm180deg - pwm0deg) / 180;

    unsigned pwmVal = pwm0deg + pwmSlope * angle;

    driver[drvNr]->setPWM(motor, 0, pwmVal);
}

Adafruit_PWMServoDriver* Pipe::driver[2] =
    {
        new Adafruit_PWMServoDriver(0x40),
        new Adafruit_PWMServoDriver(0x41)};

std::map<unsigned, const std::string> Pipe::noteNames =
    {
        {a1, "A1"},
        {ais1, "A#1"},
        {c2,"C2"},
        {f2, "F2"},
        {fis2,"F#2"},
        {a2, "A2"},
        {ais2, "A#2"},
        {g2,"G2"},
        {h2, "H2"},

        {c3, "C3"},
        {cis3, "C#3"},
        {d3, "D3"},
        {dis3, "D#3"},
        {e3, "E3"},
        {f3, "F3"},
        {fis3, "F#3"},
        {g3, "G3"},
        {gis3, "G#3"},
        {a3, "A3"},
        {ais3, "A#3"},
        {h3, "H3"},

        {c4, "C4"},
        {cis4, "C#4"},
        {d4, "D4"},
        {dis4, "D#4"},
        {e4, "E4"},
        {f4, "F4"},
        {fis4, "F#4"},
        {g4, "G4"},
        {gis4, "G#4"},
        {a4, "A4"},
        {ais4, "A#4"},
        {h4, "H4"},

        {c5, "C5"},
        {cis5, "C#5"},
        {d5, "D5"},
        {dis5, "D#5"},
        {e5, "E5"},
};
