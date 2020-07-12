#include "MidiParser.h"
#include "SdFat.h"

extern void panic();

MidiParser::MidiParser(Stream* dbg, SdFatSdioEX* sd)
{
    this->dbg = dbg;
     //sd->ls(dbg);

    SMF.begin(sd);
    SMF.setMidiHandler(midiCallback);

    dbg->println("MidiParser constructor\t- done");
}


// const std::string& MidiParser::selectNextSong(int dir)
// {
//     // curIndex+=dir;
//     // curIndex = std::max(0, std::min((int)files.size(), curIndex));

//     // if (curIndex >=(int) files.size()) curIndex = 0;
//     // else if(curIndex <0)
//     // {
//     //     curIndex = files.size() - 1;
//     // }
//     //return files[curIndex];

//     return "asdf";
// }

void MidiParser::play(const std::string& filename)
{
    SMF.close();
    std::string curFile = filename + ".mid";
    int err = SMF.load(curFile.c_str());
    dbg->printf("Play %s %s\n", curFile.c_str(), err == 0 ? "OK" : "ERR");
}

void MidiParser::stop()
{
    dbg->println("stop");
    SMF.close();
}

void MidiParser::attachOnPlayNote(midiCallback_t playNoteCb)
{
    playNoteCB = playNoteCb;
}

void MidiParser::tick()
{
    if (!SMF.isEOF())
    {
        SMF.getNextEvent();
        //dbg->println(millis());
    }
}

void MidiParser::midiCallback(midi_event* ev)
{
    constexpr int transpose = 17;

    if (ev->channel == 0) // we only play channel 0
    {
        //Serial.printf("%2X\n",ev->data[0]);
        int evType = ev->data[0];
        int note = ev->data[1] - transpose;
        int velocity = ev->data[2];

        if (evType == 0x90) // note on
        {
            playNoteCB(note, velocity != 0);
        } else if (evType == 0x80) // note off
        {
            playNoteCB(note, false);
        }
    }
}

midiCallback_t MidiParser::playNoteCB;
midiCallback_t MidiParser::offCb;