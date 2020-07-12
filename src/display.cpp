#include "display.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiSpi.h"
#include "pipe.h"
#include <algorithm>

Display::Display(unsigned disp_cs, unsigned disp_rst, unsigned disp_dc, unsigned btn_pin, unsigned enc_A, unsigned enc_B, Stream* debug)
{
    debugStream = debug;

    pinMode(btn_pin, INPUT_PULLUP);
    button = new Bounce();
    button->attach(btn_pin);

    encoder = new EncoderTool::Encoder();
    encoder->begin(enc_A, enc_B);
    encoder->attachCallback([this](int val, int delta) { this->selectionChanged(delta); });
    //{ Serial.printf("enc: val:%d delta: %d\n", val, delta); });

    SPI.setSCK(14);

    oled = new SSD1306AsciiSpi();
    oled->begin(&SH1106_128x64, disp_cs, disp_dc, disp_rst);
    oled->clear();
    oled->setScrollMode(true);

    oled->setFont(Callibri15);
    oled->println("To Pipe or Not to Pipe");

    oled->setFont(System5x7);
    oled->setCol(8);
    oled->println("-The HAMLET ORGAN-");

    oled->setFont(System5x7);
    oled->setCursor(0, 7);
    oled->print("Click button to start");

    oled->setFont(Callibri11_bold);

    state = states::idle;
    files.clear();
    pipes.clear();

    debugStream->println("Display constructor\t- done");
}

void Display::tick()
{
    encoder->tick();
    button->update();

    if (button->fell())
    {
        // debugStream->printf("Button click (state: %d)\n", state);
        switch (state)
        {
            case idle:
                if (files[curFile].compare("Settings") == 0)
                {
                    curTone = -1;
                    state = states::playing_tone;
                    selectionChanged(1);
                    //debugStream->println("switched to settings");

                } else
                {
                    playFilecb(files[curFile]);
                    state = playing_file;
                }
                break;

            case playing_tone:
                if (playingTone != -1)
                {
                    pipes[playingTone]->play(false);
                    // playingTone = -1;
                }
                if (toneExit)
                {
                    curFile = -1;
                    state = states::idle;
                    selectionChanged(1);
                } else
                {
                    if (playingTone != curTone)
                    {
                        pipes[curTone]->play(true);
                        playingTone = curTone;
                    }
                    else
                        playingTone = -1;
                }

                break;

            default:
                stopCB();
                state = idle;
                break;
        }
    }
}

void Display::addMidiFiles(const std::vector<std::string>& files)
{
    this->files = files;

    debugStream->println("addMidiFiles");
    for (auto filename : files)
    {
        debugStream->printf("   %s\n", filename.c_str());
    }
    curFile = -1;
    selectionChanged(1);
}

void Display::addSingleTones(const std::vector<Pipe*>& tones)
{
    this->pipes = tones;

    // for(auto& tone: pipes)
    // {
    //     tone->play(true);
    //     delay(500);
    //     tone->play(false);
    //     delay(500);
    // }
}

void Display::selectionChanged(int delta)
{
    if (state == states::idle)
    {
        curFile = std::min((int)files.size() - 1, std::max(0, curFile + delta));

        std::string file = files[curFile];
        if (curFile < (int)files.size())
        {
            int w = (128 - oled->strWidth(file.c_str())) / 2;
            oled->setCursor(0, 5);
            oled->clearToEOL();
            oled->setCursor(w, 5);
            // Serial.println(w);

            oled->print(file.c_str());
            debugStream->printf("%s %d\n", file.c_str(), delta);
        }
    } else if (state == states::playing_tone)
    {
        //debugStream->println(curTone);
        if (((curTone + delta) > ((int)pipes.size() - 1)) || (curTone + delta < 0))
        {
            int w = (128 - oled->strWidth("EXIT")) / 2;
            oled->setCursor(0, 5);
            oled->clearToEOL();
            oled->setCursor(w, 5);
            oled->print("EXIT");

            toneExit = true;
        } else
        {
            toneExit = false;

            curTone += delta;
            // curTone = std::min((int)pipes.size() - 1, std::max(0, curTone + delta));

            Pipe* pipe = pipes[curTone];

            int w = (128 - oled->strWidth(pipe->getName().c_str())) / 2;
            oled->setCursor(0, 5);
            oled->clearToEOL();
            oled->setCursor(w, 5);
            oled->print(pipe->getName().c_str());
        }
    }
}