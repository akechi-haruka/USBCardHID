#pragma once

// The time when the last card was read. 0 while reading is in progress / no card found.
extern long nfc_last_read;

// The UID of the last card read.
extern uint64_t nfc_last_card;

// Initializes the NFC module. Should be called in setup.
int nfc_init();

// Converts the bytes read from the NFC module to an int64.
// The setE004 flag replaces the 4 highest order bits with E004 for compability with older games.
uint64_t nfc_convert_id(byte* bytes, size_t size, bool setE004 = true);

// Attempts to read a card from the reader selected in NFCConfig.h. Returns immediately if called withing NFC_READ_DELAY.
// Does not block on RC522. Blocks on PN532/FeliCa for 100ms. (todo fix?). Returns the UID of the card read or 0. If a card
// is successfully read, the UID is also saved to nfc_last_card until the next read.
uint64_t nfc_read_card();