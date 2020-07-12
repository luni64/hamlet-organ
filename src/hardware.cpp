#include "hardware.h"
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

Display* display;
MidiParser* parser;
Organ* organ;
SdFatSdioEX* sd;

extern void panic();

namespace hardware
{
    namespace
    {
        Stream* debugStream;
        SdFile root;

        void initSD(SdFatSdioEX* sd)
        {
            if (!sd->begin())
            {
                debugStream->println("Error opening SD - Card");
                ::panic();
            }

            debugStream->println("initSD\t\t- done");
        }

        void readMidiFiles(std::vector<std::string>& files, SdFatSdioEX* sd)
        {
            SdFile file, root;
            if (!root.open("/"))
            {
                debugStream->println("Can not open root folder");
                ::panic();
            }

            while (file.openNext(&root, O_RDONLY))
            {
                char buf[50];
                if (!file.isHidden() && file.getName(buf, 50))
                {
                    std::string fullName = buf;
                    int lastDot = fullName.find_last_of('.');
                    std::string fileName = fullName.substr(0, lastDot);
                    std::string extension = fullName.substr(lastDot + 1);
                    for (char& c : extension) c = std::tolower(c);
                    if (extension.compare("mid") == 0)
                    {
                        files.push_back(fileName);
                    }
                }
                file.close();
            }
            Serial.println("readMidiFiles\t- done");
        }
    }

    void init(Stream* dbg)
    {
        debugStream = dbg;
        pinMode(LED_BUILTIN, OUTPUT);

        std::vector<std::string> midiFiles;
        sd = new SdFatSdioEX();
        initSD(sd);

        organ = new Organ(debugStream);
        parser = new MidiParser(debugStream, sd);
        //parser->begin(sd);

        readMidiFiles(midiFiles, sd);
        midiFiles.emplace_back("Settings");

        display = new Display(7, 8, 9, 0, 1, 2, debugStream); // CS, RST, D/C, BTN, ENC_A, ENC_B
        display->addMidiFiles(midiFiles);
        display->addSingleTones(organ->getAllPipes());

        // wire events:
        display->attachOnPlay([](std::string f) {organ->allOff(); parser->play(f); });    // user starts a playback
        display->attachOnStop([] {organ->allOff() ; parser->stop(); });                      // user stops a playback
        display->attachOnNoteOn([](int note) { organ->playNote(note, true); });
        display->attachOnNoteOff([](int note) { organ->playNote(note, false); });
        parser->attachOnPlayNote([](int note, bool on) { organ->playNote(note, on); });

        // auto timer = new PeriodicTimer(TCK);
        // timer->begin([] {
        //     parser->tick();
        //     display->tick();
        // },
        //              10);

        debugStream->println("hardware::init\t- done");
    }
}