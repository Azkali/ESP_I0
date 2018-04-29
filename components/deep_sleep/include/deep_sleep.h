/*
 * deep_sleep.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_DEEP_SLEEP_H_
 #define INCLUDE_DEEP_SLEEP_H_
 #ifdef __cplusplus
 extern "C" {
 #endif

 #define ESP_INTR_FLAG_DEFAULT 0

 // Pin used for pulse counting
 // GPIO33 is RTC_GPIO08 (see esp32_chip_pin_list_en.pdf)
 #define PULSE_CNT_GPIO_NUM 33
 #define PULSE_CNT_RTC_GPIO_NUM 8



//#define



 #define PULSE_CNT_IS_LOW() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(PULSE_CNT_RTC_GPIO_NUM)) == 0)

typedef struct  s_deepsleep_config{
    int pulse_button_num;
}               deepsleep_config;

 void ds_gpio_config(int pulse_button_num);
 #ifdef __cplusplus
 } //end extern "C"
 #endif
 #endif
