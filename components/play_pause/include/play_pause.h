/*
 * play_pause.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_PLAY_PAUSE_H_
 #define INCLUDE_PLAY_PAUSE_H_
 #include "driver/gpio.h"

 #define ESP_INTR_FLAG_DEFAULT 0

 // Pin used for pulse counting
 // GPIO33 is RTC_GPIO08 (see esp32_chip_pin_list_en.pdf)
 #define PULSE_CNT_GPIO_NUM_12 (gpio_num_t)12


struct BtCmdHandlerConfig {
    gpio_num_t pulse_button_num;
    
    public:
  BtCmdHandlerConfig() : pulse_button_num() {}
  BtCmdHandlerConfig(gpio_num_t btnNum) : pulse_button_num(btnNum) {};
};

template <typename T>
class BtCmdHandler {
    public:
    BtCmdHandler(T _config);

    protected:
    static void handleEvent(BtCmdHandler<T> *_this);
    virtual void handleEvent();

    private:
    T config;
};





struct PlayPauseConfig : BtCmdHandlerConfig {
    public:
    PlayPauseConfig(): BtCmdHandlerConfig(){};
    PlayPauseConfig(gpio_num_t btnNum) : BtCmdHandlerConfig(btnNum) {};
};

class BtPlayPauseHandler : public BtCmdHandler<PlayPauseConfig> {
    public:
    BtPlayPauseHandler(gpio_num_t btnNum);

    protected:
    using BtCmdHandler::handleEvent;
    void handleEvent();

    private:
    static PlayPauseConfig generateConfig(gpio_num_t btnNum);
};



 #ifdef __cplusplus
 extern "C" {
 #endif
 void play_pause_gpio_config(gpio_num_t pulse_button_num);
 #ifdef __cplusplus
 } //end extern "C"
 #endif
 #endif
