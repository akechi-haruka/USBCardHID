#include "HIDCardReader.h"

#if !defined(_USING_HID)

#error "HID library not found. You need an Arduino with USB HID support such as a Leonardo or a SparkFun Pro Micro."

#endif

// Many, many, many thanks to https://github.com/4yn/iivx/blob/master/leoo/leoo/iivx_leo.cpp for the IIVX sketch,
// and this StackOverflow answer for explaining this: https://stackoverflow.com/a/21676286
static const byte _hidReportDescriptor[] PROGMEM = {

	  0x06, 0xCA, 0xFF,            // (GLOBAL) USAGE_PAGE         0xFFCA Card Reader
	  0x09, 0x42,                  // (GLOBAL)  USAGE			  0x42 FeliCa
	  0xA1, 0x01,                  // (MAIN)   COLLECTION         0x01 Application (Usage=0x0: Page=, Usage=, Type=) 
	  0x15, 0x00,                  //   (GLOBAL) LOGICAL_MINIMUM    0x00 (0)
	  0x26, 0xFF, 0x00,            //   (GLOBAL) LOGICAL_MAXIMUM    0x00FF (255) 
	  0x75, 0x08,                  //   (GLOBAL) REPORT_SIZE        0x08 (8) Number of bits per field 
	  0x85, 0x01,                  //   (GLOBAL) REPORT_ID          0x01 (1) 
	  0x95, 0x08,                  //   (GLOBAL) REPORT_COUNT       0x08 (2) Number of fields 
	  0x09, 0x42,                  //   (LOCAL)  USAGE              0xFF000001  
	  0x81, 0x02,                  //   (MAIN)   INPUT              0x00000002 (64 fields x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
	  0x85, 0x06,                  //   (GLOBAL) REPORT_ID          0x06 (6) 
	  0x09, 0x42,                  //   (LOCAL)  USAGE              0xFF000001  
	  0x91, 0x02,                  //   (MAIN)   OUTPUT             0x00000002 (64 fields x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
	  0xC0,                        // (MAIN)   END_COLLECTION     Application
};
static const byte _hidReportDescriptor2[] PROGMEM = {

	  0x06, 0xCA, 0xFF,            // (GLOBAL) USAGE_PAGE         0xFFCA Card Reader
	  0x09, 0x42,                  // (GLOBAL)  USAGE			  0x42 FeliCa
	  0xA1, 0x01,                  // (MAIN)   COLLECTION         0x01 Application (Usage=0x0: Page=, Usage=, Type=) 
	  0x15, 0x00,                  //   (GLOBAL) LOGICAL_MINIMUM    0x00 (0)
	  0x26, 0xFF, 0x00,            //   (GLOBAL) LOGICAL_MAXIMUM    0x00FF (255) 
	  0x75, 0x08,                  //   (GLOBAL) REPORT_SIZE        0x08 (8) Number of bits per field 
	  0x85, 0x03,                  //   (GLOBAL) REPORT_ID          0x03 (3) 
	  0x95, 0x08,                  //   (GLOBAL) REPORT_COUNT       0x08 (2) Number of fields 
	  0x09, 0x42,                  //   (LOCAL)  USAGE              0xFF000001  
	  0x81, 0x02,                  //   (MAIN)   INPUT              0x00000002 (64 fields x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
	  0x85, 0x07,                  //   (GLOBAL) REPORT_ID          0x07 (7) 
	  0x09, 0x42,                  //   (LOCAL)  USAGE              0xFF000001  
	  0x91, 0x02,                  //   (MAIN)   OUTPUT             0x00000002 (64 fields x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
	  0xC0,                        // (MAIN)   END_COLLECTION     Application
};

bool card_reader_initstate[2];

int hid_card_init(int index) {

	if (card_reader_initstate[index]) {
		return CARD_READER_FAILURE_ALREADY_INITIALIZED;
	}

	if (index > 0) {
		static HIDSubDescriptor node(_hidReportDescriptor2, sizeof(_hidReportDescriptor2));
		HID().AppendDescriptor(&node);
	} else {
		static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
		HID().AppendDescriptor(&node);
	}

	card_reader_initstate[index] = true;

	return CARD_READER_SUCCESS;
}

int hid_card_send(int index, uint64_t card) {

	if (!card_reader_initstate[index]) {
		return CARD_READER_FAILURE_UNINITIALIZED;
	}
	
	uint8_t data[8];
	for (int i = 7; i >= 0; i--) {
		data[i] = card & 0xFF;
		card >>= 8;
	}

	int res = HID().SendReport(index > 0 ? 3 : 1, data, sizeof(data));
	if (res < 0) {
		return CARD_READER_FAILURE_SEND_REPORT;
	}

	return CARD_READER_SUCCESS;
}