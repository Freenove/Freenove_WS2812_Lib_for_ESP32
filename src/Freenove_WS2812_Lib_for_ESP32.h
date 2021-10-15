// Freenove_WS2812_Lib_for_ESP32.h
/**
 * Brief	A library for controlling ws2812 in esp32 platform.
 * Author	ZhentaoLin
 * Company	Freenove
 * Date		2021-10-15
 */

#ifndef _FREENOVE_WS2812_LIB_FOR_ESP32_h
#define _FREENOVE_WS2812_LIB_FOR_ESP32_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"

#include "esp32-hal.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

//Modify the definition to expand the number of leds
//Supports a maximum of 1100 leds
#define NR_OF_LEDS   256  

#define NR_OF_ALL_BITS 24*NR_OF_LEDS

enum LED_TYPE
{					  //R  G  B
	TYPE_RGB = 0x06,  //00 01 10
	TYPE_RBG = 0x09,  //00 10 01
	TYPE_GRB = 0x12,  //01 00 10
	TYPE_GBR = 0x21,  //10 00 01
	TYPE_BRG = 0x18,  //01 10 00
	TYPE_BGR = 0x24	  //10 01 00
};

class Freenove_ESP32_WS2812
{
protected:
	
	u16 ledCounts;
	u8 pin;
	u8 br;
	u8 rmt_chn;
	
	u8 rOffset;
	u8 gOffset;
	u8 bOffset;
	
	float realTick;
	rmt_reserve_memsize_t rmt_mem;
	rmt_data_t led_data[NR_OF_ALL_BITS];
	rmt_obj_t* rmt_send = NULL;

public:
	Freenove_ESP32_WS2812(u16 n = 8, u8 pin_gpio = 2, u8 chn = 0, LED_TYPE t = TYPE_GRB);

	bool begin();
	void setLedCount(u16 n);
	void setLedType(LED_TYPE t);
	void setBrightness(u8 brightness);

	esp_err_t set_pixel(int index, u8 r, u8 g, u8 b);
	
	esp_err_t setLedColorData(int index, u32 rgb);
	esp_err_t setLedColorData(int index, u8 r, u8 g, u8 b);

	esp_err_t setLedColor(int index, u32 rgb);
	esp_err_t setLedColor(int index, u8 r, u8 g, u8 b);

	esp_err_t setAllLedsColorData(u32 rgb);
	esp_err_t setAllLedsColorData(u8 r, u8 g, u8 b);

	esp_err_t setAllLedsColor(u32 rgb);
	esp_err_t setAllLedsColor(u8 r, u8 g, u8 b);

	esp_err_t show();

	uint32_t Wheel(byte pos);
	uint32_t hsv2rgb(uint32_t h, uint32_t s, uint32_t v);
};

#endif

