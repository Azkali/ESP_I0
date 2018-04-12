/*
 * deep_sleep.h
 *
 *  Created on: 31.03.2018
 *      Author: Azkali Manad
 */
 #ifndef INCLUDE_PLAY_PAUSE_H_
 #define INCLUDE_PLAY_PAUSE_H_

 #define PULSE_CNT_GPIO_NUM 16
 //#define



 #define PULSE_CNT_IS_LOW() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(PULSE_CNT_RTC_GPIO_NUM)) == 0)

typedef struct  s_play_pause_config{
    int pulse_button_num;
}               play_pause_config;

void ds_gpio_config(int pulse_button_num);

 #endif
