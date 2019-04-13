// HIDCardReader.h

#ifndef _HIDCARDREADER_h
#define _HIDCARDREADER_h

#include <Arduino.h>
#include <HID.h>

#define CARD_READER_FAILURE_NOT_FOUND -4
#define CARD_READER_FAILURE_SEND_REPORT -3
#define CARD_READER_FAILURE_ALREADY_INITIALIZED -2
#define CARD_READER_FAILURE_UNINITIALIZED -1
#define CARD_READER_SUCCESS 0

// Initializes the HID communication to the PC. Returns CARD_READER_SUCCESS on success.
// Must be called in setup as soon as possible. If no devices show up, either called too late or driver issue.
// See "Devices and Printers" for more information.
// index can be 0 or 1.
int hid_card_init(int index);

// Sends a card UID to the PC/SpiceTools/etc. Returns CARD_READER_SUCCESS on success.
// index can be 0 or 1. hid_card_init must have been called for the reader at the specified index beforehand.
int hid_card_send(int index, uint64_t cardid);


#endif
