# Author: diplfranzhoepfinger 
# reference:  https://docs.espressif.com/projects/arduino-esp32/en/latest/esp-idf_component.html
# URL: https://github.com/Freenove/Freenove_WS2812_Lib_for_ESP32
# DATE: 15.02.2023

idf_component_register(
    SRCS "src/Freenove_WS2812_Lib_for_ESP32.cpp"
    INCLUDE_DIRS "src"
    PRIV_REQUIRES arduino
    )
