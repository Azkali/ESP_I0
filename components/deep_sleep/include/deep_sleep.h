/*
 * deep_sleep.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_DEEP_SLEEP_H_
 #define INCLUDE_DEEP_SLEEP_H_

 // Pin used for pulse counting
 // GPIO33 is RTC_GPIO08 (see esp32_chip_pin_list_en.pdf)
 #define PULSE_CNT_GPIO_NUM_33 GPIO_NUM_12
 #define PULSE_CNT_RTC_GPIO_NUM (gpio_num_t)8
 #include "btnCmdHandler.h"
 #include "driver/gpio.h"
struct DeepSleepConfig : public BtnCmdHandlerConfig {
    public:
    DeepSleepConfig(): BtnCmdHandlerConfig(){};
    DeepSleepConfig(gpio_num_t btnNum) : BtnCmdHandlerConfig(btnNum) {};
};

class DeepSleepHandler : public SimpleBtnCmdHandler {
    public:
    DeepSleepHandler(gpio_num_t pulseButtonNum);
    void handleEvent();
};


 #define PULSE_CNT_IS_LOW() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(PULSE_CNT_RTC_GPIO_NUM)) == 0)

 #ifdef __cplusplus
 extern "C" {
 #endif
 void ds_gpio_config(gpio_num_t pulse_button_num);
 #ifdef __cplusplus
 } //end extern "C"
 #endif
 #endif