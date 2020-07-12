#include "hardware.h"

// void printNextFile(int val, int delta){
//     // static std::string oldFile = "";
//     // std::string file = parser.selectNextSong(delta);
//     // if (oldFile != file)
//     // {
//     //     oldFile = file;

//     //     int w = (128 - oled.strWidth(file.c_str())) / 2;
//     //     oled.setCursor(0, 5);
//     //     oled.clearToEOL();
//     //     oled.setCursor(w, 5);
//     //     Serial.println(w);

//     //     oled.print(file.c_str());
//     //     Serial.printf("%s %d\n", file.c_str(), delta);
//     // }
// };

enum states {
    idle,
    playing,
} state;

SdFile root, file;

void setup(void)
{
    while (!Serial && millis() < 1000) {}
    Serial.println("Hamlet Organ ----------------");
    hardware::init(&Serial);

    Serial.println("Setup\t\t- done");
    Serial.println("-----------------------------");

    constexpr int note = ais2;

    organ->allOff();

    parser->play("Paula's Hamlet-Zoo3");
}

elapsedMillis sw = 0;
void loop(void)
{
    parser->tick();
    display->tick();

    // enc.tick();
    // button.update();

    // if (button.fell())
    // {
    //     switch (state)
    //     {
    //         case idle:
    //             // organ.allOff();
    //             parser.play("");
    //             state = playing;
    //             break;

    //         default:
    //             parser.stop();
    //             //   organ.allOn();
    //             state = idle;
    //             break;
    //     }
    // }

    if (sw > 200)
    {
        sw -= 200;
        digitalToggleFast(LED_BUILTIN);
    }
}

void panic()
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (1)
    {
        digitalToggle(LED_BUILTIN);
        delay(50);
    }
}