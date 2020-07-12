#include "Organ.h"
#include <algorithm>
#include <tuple>

constexpr unsigned nrPipes = 8;

Organ::Organ(Stream* dbg)
    : pwm0(0x40), pwm1(0x41), debugStream(dbg)
{
    debugStream->print("Organ constructor, ");

    pwm0.begin();
    pwm1.begin();

    pwm0.setOscillatorFrequency(26'930'000);
    pwm0.setPWMFreq(50);

    pwm1.setOscillatorFrequency(26'930'000);
    pwm1.setPWMFreq(50);

    Wire.setClock(1'000'000);

    pipes.push_back(new Pipe(a2,  1, 8, 70));
    pipes.push_back(new Pipe(ais2,0, 7, 55));
    pipes.push_back(new Pipe(h2,  1, 9, 75));

    pipes.push_back(new Pipe(c3,  0, 6,  35));
    pipes.push_back(new Pipe(d3,  1, 10, 68));
    pipes.push_back(new Pipe(e3,  0, 5,  35));
    pipes.push_back(new Pipe(f3,  1, 11, 70));
    pipes.push_back(new Pipe(g3,  0, 4,  55));
    pipes.push_back(new Pipe(a3,  1, 12, 105));
    pipes.push_back(new Pipe(ais3,0, 3,  30));
    pipes.push_back(new Pipe(h3,  1, 13, 105));

    pipes.push_back(new Pipe(c4,  0, 2,  50));
    pipes.push_back(new Pipe(d4,  1, 14, 90));
    pipes.push_back(new Pipe(e4,  0, 1,  55));
    pipes.push_back(new Pipe(f4,  1, 15, 75));
    pipes.push_back(new Pipe(g4,  0, 0,  50));


    debugStream->println("pipes:");
    debugStream->println("   MIDI | NOTE | MOTOR");
    for (auto* note : pipes)
    {
        debugStream->printf("   %4d |%5s | M%d-%d\n", note->getMidiNr(), note->getName().c_str(), note->getPWM(), note->getMotorNr());
    }
    debugStream->println("Organ constructor\t- done");
}

void Organ::playNote(unsigned midiNr, bool on)
{
    //Serial.println("playNote");
    auto it = std::find_if(pipes.begin(), pipes.end(), [midiNr](auto& pipe) { return pipe->getMidiNr() == midiNr; });

    if (it != pipes.end())
    {
        (*it)->play(on);
    } else
    {
        auto it = Pipe::noteNames.find(midiNr);
        if (it != Pipe::noteNames.end())
        {
            debugStream->printf("Pipe %d (%3s) not found\n", midiNr, Pipe::noteNames[midiNr].c_str());
        } else
            debugStream->printf("Pipe %d (%3s) not found\n", midiNr, "XX");
    }
}

void Organ::allOff()
{
    for (auto& pipe : pipes)
    {
        pipe->play(false);
    }
}

void Organ::allOn()
{
    for (auto& pipe : pipes)
    {
        pipe->play(true);
    }
}
