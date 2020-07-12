// To give your project a unique name, this code must be
// placed into a .c file (its own tab).  It can not be in
// a .cpp file or your main sketch (the .ino file).

#include "usb_names.h"

// Edit these lines to create your own name.  The length must
// match the number of characters in your custom name.

#define MANUFACTURER_NAME   {'N','i','S','o','f','t'}
#define MANUFACTURER_NAME_LEN  6
#define MIDI_NAME   {'H','A','M','L','E','T',' ','M','A','S','C','H','I','N','E'}
#define MIDI_NAME_LEN  15
// #define SERIAL_NUM {'E','n','c','o','d','e','r','P','a','n','e','l'}
// #define SERIAL_NUM_LEN 12

// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
        2 + MANUFACTURER_NAME_LEN * 2,
        3,
        MANUFACTURER_NAME
};

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};