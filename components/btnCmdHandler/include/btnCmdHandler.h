/*
 * btnCmdHandler.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_BTN_CMD_HANDLER_H_
 #define INCLUDE_BTN_CMD_HANDLER_H_
 #include "driver/gpio.h"
 #include "esp_log.h"


 struct BtCmdHandlerConfig {
    gpio_num_t pulse_button_num;
    
    public:
    BtCmdHandlerConfig() : pulse_button_num() {};
    BtCmdHandlerConfig(gpio_num_t btnNum) : pulse_button_num(btnNum) {};
};

template <typename Tconfig, class Tinherit>
class BtCmdHandler {
    public:
    BtCmdHandler(Tconfig _config);
    virtual void handleEvent() = 0;

    protected:
    static void handleEventBetter(Tinherit *_this);

    private:
    Tconfig config;
};

 #ifdef __cplusplus
 extern "C" {
 #endif
 #ifdef __cplusplus
 } //end extern "C"
 #endif
 #endif