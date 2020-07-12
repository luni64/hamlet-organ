#pragma once

#include "Bounce2.h"
#include "EncoderTool.h"
#include "Stream.h"
#include <functional>
#include <string>
#include <vector>

class Pipe;

class SSD1306AsciiSpi;

class Display
{
 public:
    Display(unsigned disp_cs, unsigned disp_rst, unsigned disp_dc, unsigned btn_pin, unsigned enc_A, unsigned enc_B, Stream* debugStream);
    void begin(unsigned disp_cs, unsigned disp_rst, unsigned disp_dc, unsigned btn_pin, unsigned enc_A, unsigned enc_B);

    void addMidiFiles(const std::vector<std::string>& files);
    void addSingleTones(const std::vector<Pipe*>&);

    void attachOnPlay(std::function<void(std::string)> cb) { playFilecb = cb; }
    void attachOnStop(std::function<void(void)> cb) { stopCB = cb; }
    void attachOnNoteOn(std::function<void(int)> cb) { noteOnCB = cb; }
    void attachOnNoteOff(std::function<void(int)> cb) { noteOffCB = cb; }

    void tick();

 protected:
    int curFile = -1;
    int curTone = -1;
    bool toneExit = true;
    int playingTone = -1;
    void selectionChanged(int delta);

    SSD1306AsciiSpi* oled;
    std::vector<std::string> files;
    std::vector<Pipe*> pipes;
    Bounce* button;
    EncoderTool::Encoder* encoder;

    std::function<void(std::string)> playFilecb;
    std::function<void(void)> stopCB;
    std::function<void(int)> noteOnCB;
    std::function<void(int)> noteOffCB;

    enum states {
        idle,
        playing_file,
        playing_tone,
    } state;

    Stream* debugStream;
};
