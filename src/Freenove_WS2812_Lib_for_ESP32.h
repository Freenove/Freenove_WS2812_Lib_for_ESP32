// Freenove_WS2812_Lib_for_ESP32.h
/**
 * Brief	A library for controlling ws2812 in esp32 platform.
 * Author	ZhentaoLin
 * Company	Freenove
 * Date		2024-02-29
 *
 * Dual RMT backend:
 *   - arduino-esp32 2.0.x (ESP-IDF 4.x): legacy rmtInit/rmtWrite/rmt_data_t
 *   - arduino-esp32 3.x   (ESP-IDF 5.x): new rmt_new_tx_channel/rmt_bytes_encoder/rmt_transmit
 * Selected automatically by ESP_IDF_VERSION_MAJOR. Public class API is identical on both.
 */

#ifndef _FREENOVE_WS2812_LIB_FOR_ESP32_h
#define _FREENOVE_WS2812_LIB_FOR_ESP32_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

#include "esp_idf_version.h"

#if defined(ESP_IDF_VERSION_MAJOR) && (ESP_IDF_VERSION_MAJOR >= 5)
// ---- ESP-IDF 5.x: new RMT driver ----
#include "esp_err.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#else
// ---- ESP-IDF 4.x: legacy Arduino RMT wrapper ----
#include "esp32-hal.h"
#endif

// Pixel buffer is allocated dynamically to match the configured LED count
// (set via the constructor's 'n' argument, default 8). No fixed maximum.

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
	
	uint16_t ledCounts;
	uint8_t pin;
	uint8_t br;
	uint8_t rmt_chn;
	
	uint8_t rOffset;
	uint8_t gOffset;
	uint8_t bOffset;

	bool inited;
#if defined(ESP_IDF_VERSION_MAJOR) && (ESP_IDF_VERSION_MAJOR >= 5)
	// ---- ESP-IDF 5.x backend members ----
	rmt_channel_handle_t rmt_chan;
	rmt_encoder_handle_t rmt_enc;
	uint8_t *pixels;        // dynamically sized: ledCounts * 3
#else
	// ---- ESP-IDF 4.x backend members ----
	rmt_reserve_memsize_t rmt_mem;
	rmt_data_t *led_data;   // dynamically sized: ledCounts * 24
#endif

public:
	Freenove_ESP32_WS2812(uint16_t n = 8, uint8_t pin_gpio = 2, uint8_t chn = 0, LED_TYPE t = TYPE_GRB);
	~Freenove_ESP32_WS2812();

	bool begin();
	void setLedCount(uint16_t n);
	void setLedType(LED_TYPE t);
	void setBrightness(uint8_t brightness);

	esp_err_t set_pixel(int index, uint8_t r, uint8_t g, uint8_t b);
	
	esp_err_t setLedColorData(int index, uint32_t rgb);
	esp_err_t setLedColorData(int index, uint8_t r, uint8_t g, uint8_t b);

	esp_err_t setLedColorAndBrightnessData(int index, uint32_t rgb,  uint8_t  brightness);
	esp_err_t setLedColorAndBrightnessData(int index, uint8_t  r, uint8_t  g, uint8_t b,  uint8_t  brightness);

	esp_err_t setLedColor(int index, uint32_t rgb);
	esp_err_t setLedColor(int index, uint8_t r, uint8_t g, uint8_t b);

	esp_err_t setAllLedsColorData(uint32_t rgb);
	esp_err_t setAllLedsColorData(uint8_t r, uint8_t g, uint8_t b);

	esp_err_t setAllLedsColor(uint32_t rgb);
	esp_err_t setAllLedsColor(uint8_t r, uint8_t g, uint8_t b);

	esp_err_t show();

	uint32_t Wheel(uint8_t pos);
	uint32_t hsv2rgb(uint32_t h, uint32_t s, uint32_t v);
};

#endif
