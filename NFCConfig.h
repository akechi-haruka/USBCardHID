#pragma once
#ifndef USB_FELICA_CONFIG_H
#define USB_FELICA_CONFIG_H

#define NFC_READER_RC522 1
#define NFC_READER_PN532 2
#define NFC_PN532_I2C 1
#define NFC_PN532_SPI 2

// setting [ NFC_READER_RC522, NFC_READER_PN532 ]
#define NFC_READER_CHOICE NFC_READER_PN532

#if NFC_READER_CHOICE == NFC_READER_RC522

// setting [int; pin number]
#define RC522_SS_PIN 53
// setting [int; pin number]
#define RC522_RST_PIN 5

#elif NFC_READER_CHOICE == NFC_READER_PN532

// setting [ NFC_PN532_I2C; NFC_PN532_SPI ]
#define NFC_PN532_COMM_MODE NFC_PN532_I2C

#if NFC_PN532_COMM_MODE == NFC_PN532_SPI

// setting [int; pin number]
#define NFC_PN532_SS_PIN 53

#endif


#endif

// setting [int]
#define NFC_READ_DELAY 500

// setting [0~1]
#define CARD_READER_INDEX 0

#endif