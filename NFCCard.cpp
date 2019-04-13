#include "NFCConfig.h"
#include "HIDCardReader.h"

// PN532 card reading from https://qiita.com/gpioblink/items/91597a5275862f7ffb3c (Japanese)

long nfc_last_read = 0;
uint64_t nfc_last_card = 0;

#if NFC_READER_CHOICE == NFC_READER_RC522

#include <require_cpp11.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <SPI.h>
#include <MFRC522.h>
#include "HIDCardReader.h"
MFRC522 rfid(RC522_SS_PIN, RC522_RST_PIN);

#elif NFC_READER_CHOICE == NFC_READER_PN532

#include <PN532.h>

#if NFC_PN532_COMM_MODE == NFC_PN532_SPI
#include <SPI.h>
#include <PN532_SPI.h>
PN532_SPI pn532spi(SPI, NFC_PN532_SS_PIN);
PN532 nfc(pn532spi);
#elif NFC_PN532_COMM_MODE == NFC_PN532_I2C
#include <PN532_I2C.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif

#endif

int nfc_init() {
#if NFC_READER_CHOICE == NFC_READER_RC522
	SPI.begin();
	rfid.PCD_Init();
	return CARD_READER_SUCCESS;
#elif NFC_READER_CHOICE == NFC_READER_PN532
	nfc.begin();

	uint32_t versiondata = nfc.getFirmwareVersion();
	if (!versiondata) {
		return CARD_READER_FAILURE_NOT_FOUND;
	}

	nfc.setPassiveActivationRetries(0xFF);
	nfc.SAMConfig();

	return CARD_READER_SUCCESS;

#endif
}


uint64_t nfc_convert_id(byte* bytes, size_t size, bool setE004 = true) {
	uint64_t res = 0;
	for (int i = 0; i < size - 2; i++) {
		res |= (static_cast<uint64_t>(bytes[i]) << (i * 8));
	}
	if (setE004) {
		res |= static_cast<uint64_t>(0x04) << ((size - 2) * 8);
		res |= static_cast<uint64_t>(0xE0) << ((size - 1) * 8);
	} else {
		for (int i = size - 2; i < size; i++) {
			res |= (static_cast<uint64_t>(bytes[i]) << (i * 8));
		}
	}
	return res;
}

uint64_t nfc_read_card() {

	if (millis() < nfc_last_read + NFC_READ_DELAY) {
		return nfc_last_card;
	}
	nfc_last_read = millis();
	nfc_last_card = 0;

#if NFC_READER_CHOICE == NFC_READER_RC522
	if (!rfid.PICC_IsNewCardPresent()) {
		return 0;
	}

	if (!rfid.PICC_ReadCardSerial()) {
		return 0;
	}

	printHex(rfid.uid.uidByte, rfid.uid.size);

	rfid.PICC_HaltA();

	nfc_last_card = nfc_convert_id(rfid.uid.uidByte, rfid.uid.size);
	return nfc_last_card;
#elif NFC_READER_CHOICE == NFC_READER_PN532
	uint16_t systemCode = 0xFE00;
	uint8_t requestCode = 0x01;
	uint8_t idm[8];
	uint8_t pmm[8];
	uint16_t systemCodeResponse;
	uint8_t ret = nfc.felica_Polling(systemCode, requestCode, idm, pmm, &systemCodeResponse, 100);
	if (ret != 1) {
		return nfc_last_card;
	}

	nfc_last_card = nfc_convert_id(idm, 8);
	return nfc_last_card;
#endif
}
