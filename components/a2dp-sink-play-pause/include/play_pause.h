/*
 * play_pause.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_PLAY_PAUSE_H_
 #define INCLUDE_PLAY_PAUSE_H_

 // Pin used for pulse counting
 // GPIO33 is RTC_GPIO08 (see esp32_chip_pin_list_en.pdf)
 #define PULSE_CNT_GPIO_NUM_12 GPIO_NUM_33
 #include "btnCmdHandler.h"

struct PlayPauseConfig : public BtnCmdHandlerConfig {
    public:
    PlayPauseConfig(): BtnCmdHandlerConfig(){};
    PlayPauseConfig(gpio_num_t btnNum) : BtnCmdHandlerConfig(btnNum) {};
};


class BtnPlayPauseHandler : public SimpleBtnCmdHandler {
    public:
    BtnPlayPauseHandler(gpio_num_t pulseButtonNum);
    void handleEvent();
};



 #ifdef __cplusplus
 extern "C" {
 #endif
 void play_pause_gpio_config(gpio_num_t pulse_button_num);
 #ifdef __cplusplus
 } //end extern "C"
 #endif
 #endif
