/*
USB HID Card Reader Sketch
(c) 2019 Haruka Kyoubate

Libraries needed:
https://github.com/elechouse/PN532 (PN532 for FeliCa)
OR
https://github.com/miguelbalboa/rfid (RC522 for Generic)

Change settings in NFCConfig.h

*/
/*
	This file is part of USBCardHID.

	USBCardHID is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with USBCardHID.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <PN532.h>
#include <PN532_I2C.h>
#include <SPI.h>
#include <PN532_SPI.h>

#include <Wire.h>
#include "NFCConfig.h"
#include "HIDCardReader.h"
#include "NFCCard.h"

void setup() {
	Serial.begin(9600);

	hid_card_init(CARD_READER_INDEX);
	//hid_card_init(0);
	//hid_card_init(1);

	nfc_init();
}

void loop() {

	uint64_t card = nfc_read_card();
	if (card){
		hid_card_send(CARD_READER_INDEX, card);
	}

	delay(100);
}