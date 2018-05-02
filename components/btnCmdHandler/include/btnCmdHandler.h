/*
 * btnCmdHandler.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
  #ifndef INCLUDE_BTN_CMD_HANDLER_H_
 #define INCLUDE_BTN_CMD_HANDLER_H_
 #include "GPIO.h"
 #include "esp_log.h"

 #include "FreeRTOS.h"
 #include "freertos/task.h"

 #define ESP_INTR_FLAG_DEFAULT 0

 struct BtnCmdHandlerConfig {
    gpio_num_t pulse_button_num;
    
    public:
    BtnCmdHandlerConfig() : pulse_button_num() {};
    BtnCmdHandlerConfig(gpio_num_t btnNum) : pulse_button_num(btnNum) {};
};

template <typename Tconfig, class Tinherit>
class BtnCmdHandler {
    public:
    BtnCmdHandler(Tconfig _config);
    virtual void handleEvent() = 0;
    static void handleEventBetter(Tinherit *_this);
    virtual void buttonBind();

    protected:
    Tconfig config;

    private:
    static bool isrIsInstalled;
};


class SimpleBtnCmdHandler : public BtnCmdHandler<BtnCmdHandlerConfig, SimpleBtnCmdHandler> {
    public:
    SimpleBtnCmdHandler(gpio_num_t buttonNum);
    
    private:
    static BtnCmdHandlerConfig generateConfig(gpio_num_t btnNum);
};

 #endif 