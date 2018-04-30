/*
 * deep_sleep.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_DEEP_SLEEP_H_
 #define INCLUDE_DEEP_SLEEP_H_


 #define ESP_INTR_FLAG_DEFAULT 0

 // Pin used for pulse counting
 // GPIO33 is RTC_GPIO08 (see esp32_chip_pin_list_en.pdf)
 #define PULSE_CNT_GPIO_NUM_33 (gpio_num_t)33
 #define PULSE_CNT_RTC_GPIO_NUM 8
 #include "btnCmdHandler.h"

struct DeepSleepConfig : BtCmdHandlerConfig {
    public:
    DeepSleepConfig(): BtCmdHandlerConfig(){};
    DeepSleepConfig(gpio_num_t btnNum) : BtCmdHandlerConfig(btnNum) {};
};

class DeepSleepHandler : public BtCmdHandler<DeepSleepConfig, DeepSleepHandler> {
    public:
    DeepSleepHandler(gpio_num_t btnNum);
    void handleEvent();

    protected:

    private:
    static DeepSleepConfig generateConfig(gpio_num_t btnNum);
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
