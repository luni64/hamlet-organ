#pragma once

#include "MidiParser.h"
#include "Organ.h"
#include "display.h"

namespace hardware
{
    extern void init(Stream* debugStream);
}

extern Display* display;
extern MidiParser* parser;
extern Organ* organ;
extern SdFatSdioEX* sd;