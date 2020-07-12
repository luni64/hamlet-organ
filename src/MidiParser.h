#pragma once

#include "MD_MIDIFile.h"
#include <functional>
// #include <string>
// #include <vector>

using midiCallback_t = std::function<void(unsigned,bool)>;

class MidiParser
{
 public:
    MidiParser(Stream* debug, SdFatSdioEX*);

    void begin(SdFatSdioEX*);
    void attachOnPlayNote(midiCallback_t note);
    void tick();

    void play(const std::string& filename);
    void stop();
    const std::string& selectNextSong(int dir);

 protected:
    Stream* dbg;
    MD_MIDIFile SMF;
    //int curIndex = -1;

    //std::vector<std::string> files;

    //void mev(midi_event*);

    static midiCallback_t playNoteCB;
    static midiCallback_t offCb;
    static void midiCallback(midi_event* event);
};