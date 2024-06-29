// 
// 
// 
/**
 * Brief	A library for controlling ws2812 in esp32 platform.
 * Author	ZhentaoLin
 * Company	Freenove
 * Date		2024-02-29
 */

#include "Freenove_WS2812_Lib_for_ESP32.h"

Freenove_ESP32_WS2812::Freenove_ESP32_WS2812(uint16_t n /*= 8*/, uint8_t pin_gpio /*= 2*/, uint8_t chn /*= 0*/, LED_TYPE t /*= TYPE_GRB*/)
{
	ledCounts = n;
	pin = pin_gpio;
	rmt_chn = chn;
	//rmt_mem = RMT_MEM_64;
	rmt_mem = RMT_MEM_NUM_BLOCKS_1;
	br = 255;
	setLedType(t);
}

bool Freenove_ESP32_WS2812::begin()
{	
	switch(rmt_chn){
		case 0:
			//rmt_mem=RMT_MEM_64;break;
			rmt_mem=RMT_MEM_NUM_BLOCKS_1;
			break;
		case 1:
			//rmt_mem=RMT_MEM_128;break;
			rmt_mem=RMT_MEM_NUM_BLOCKS_2;
			break;
		case 2:
			//rmt_mem=RMT_MEM_192;break;
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 2
			rmt_mem=RMT_MEM_NUM_BLOCKS_3;
		#endif
			break;
		case 3:
			//rmt_mem=RMT_MEM_256;break;
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 2
			rmt_mem=RMT_MEM_NUM_BLOCKS_4;
		#endif
			break;
		case 4:
			//rmt_mem=RMT_MEM_320;break;
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 4
			rmt_mem=RMT_MEM_NUM_BLOCKS_5;
		#endif
			break;
		case 5:
			//rmt_mem=RMT_MEM_384;break;
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 4
			rmt_mem=RMT_MEM_NUM_BLOCKS_6;
		#endif
			break;
		case 6:
			//rmt_mem=RMT_MEM_448;break;
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 4
			rmt_mem=RMT_MEM_NUM_BLOCKS_7;
		#endif
			break;
		case 7:
			//rmt_mem=RMT_MEM_512;break;	
		#if SOC_RMT_TX_CANDIDATES_PER_GROUP > 4
			rmt_mem=RMT_MEM_NUM_BLOCKS_8;
		#endif
			break;	
		default:
			//rmt_mem=RMT_MEM_64;break;
			rmt_mem=RMT_MEM_NUM_BLOCKS_1;break;
	}
	//if ((rmt_send = rmtInit(pin, true, rmt_mem)) == NULL){
	if(rmtInit(pin, RMT_TX_MODE, rmt_mem, 10000000)==false){
		return false;
	}
	for(int i=0;i<ledCounts;i++)
	{
		for (int bit = 0; bit < 24; bit++) {
			led_data[i*24+bit].level0 = 1;
			led_data[i*24+bit].duration0 = 4;
			led_data[i*24+bit].level1 = 0;
			led_data[i*24+bit].duration1 = 8;
		}
	}
	//realTick = rmtSetTick(rmt_send, 100);
	return true;
}

void Freenove_ESP32_WS2812::setLedCount(uint16_t n)
{
	ledCounts = n;
	//rmtDeinit(rmt_send); // Deinitialize the RMT channel before begin() initilizes RMT channel again.
	begin();
}

void Freenove_ESP32_WS2812::setLedType(LED_TYPE t)
{
	rOffset = (t >> 4) & 0x03;
	gOffset = (t >> 2) & 0x03;
	bOffset = t & 0x03;
}

void Freenove_ESP32_WS2812::setBrightness(uint8_t brightness)
{
	br = constrain(brightness, 0, 255);
}

esp_err_t Freenove_ESP32_WS2812::setLedColorData(int index, uint32_t rgb)
{
	return setLedColorData(index, rgb >> 16, rgb >> 8, rgb);
}

esp_err_t Freenove_ESP32_WS2812::setLedColorData(int index, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t p[3];
	p[rOffset] = r * br / 255;
	p[gOffset] = g * br / 255;
	p[bOffset] = b * br / 255;
	return set_pixel(index, p[0], p[1], p[2]);
}

esp_err_t Freenove_ESP32_WS2812::set_pixel(int index, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t color = r << 16 | g << 8 | b ;
	for (int bit = 0; bit < 24; bit++) {
		if (color & (1 << (23-bit))) {
			led_data[index*24+bit].level0 = 1;
			led_data[index*24+bit].duration0 = 8;
			led_data[index*24+bit].level1 = 0;
			led_data[index*24+bit].duration1 = 4;
		} else {
			led_data[index*24+bit].level0 = 1;
			led_data[index*24+bit].duration0 = 4;
			led_data[index*24+bit].level1 = 0;
			led_data[index*24+bit].duration1 = 8;
		}
	}
	return ESP_OK;
}

esp_err_t Freenove_ESP32_WS2812::setLedColor(int index, uint32_t rgb)
{
	return setLedColor(index, rgb >> 16, rgb >> 8, rgb);
}

esp_err_t Freenove_ESP32_WS2812::setLedColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
	setLedColorData(index, r, g, b);
	return show();
}

esp_err_t Freenove_ESP32_WS2812::setAllLedsColorData(uint32_t rgb)
{
	for (int i = 0; i < ledCounts; i++)
	{
		setLedColorData(i, rgb);
	}
	return ESP_OK;
}

esp_err_t Freenove_ESP32_WS2812::setAllLedsColorData(uint8_t r, uint8_t g, uint8_t b)
{
	for (int i = 0; i < ledCounts; i++)
	{
		setLedColorData(i, r, g, b);
	}
	return ESP_OK;
}

esp_err_t Freenove_ESP32_WS2812::setAllLedsColor(uint32_t rgb)
{
	setAllLedsColorData(rgb);
	show();
	return ESP_OK;
}

esp_err_t Freenove_ESP32_WS2812::setAllLedsColor(uint8_t r, uint8_t g, uint8_t b)
{
	setAllLedsColorData(r, g, b);
	show();
	return ESP_OK;
}

esp_err_t Freenove_ESP32_WS2812::show()
{
	//return rmtWrite(rmt_send, led_data, ledCounts*24);
	return rmtWrite(pin, led_data, ledCounts*24, RMT_WAIT_FOR_EVER);
}

uint32_t Freenove_ESP32_WS2812::Wheel(byte pos)
{
	uint32_t WheelPos = pos % 0xff;
	if (WheelPos < 85) {
		return ((255 - WheelPos * 3) << 16) | ((WheelPos * 3) << 8);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return (((255 - WheelPos * 3) << 8) | (WheelPos * 3));
	}
	WheelPos -= 170;
	return ((WheelPos * 3) << 16 | (255 - WheelPos * 3));
}

uint32_t Freenove_ESP32_WS2812::hsv2rgb(uint32_t h, uint32_t s, uint32_t v)
{
	uint8_t r, g, b;
	h %= 360; // h -> [0,360]
	uint32_t rgb_max = v * 2.55f;
	uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

	uint32_t i = h / 60;
	uint32_t diff = h % 60;

	// RGB adjustment amount by hue
	uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

	switch (i) {
	case 0:
		r = rgb_max;
		g = rgb_min + rgb_adj;
		b = rgb_min;
		break;
	case 1:
		r = rgb_max - rgb_adj;
		g = rgb_max;
		b = rgb_min;
		break;
	case 2:
		r = rgb_min;
		g = rgb_max;
		b = rgb_min + rgb_adj;
		break;
	case 3:
		r = rgb_min;
		g = rgb_max - rgb_adj;
		b = rgb_max;
		break;
	case 4:
		r = rgb_min + rgb_adj;
		g = rgb_min;
		b = rgb_max;
		break;
	default:
		r = rgb_max;
		g = rgb_min;
		b = rgb_max - rgb_adj;
		break;
	}
	return (uint32_t)(r << 16 | g << 8 | b);
}
