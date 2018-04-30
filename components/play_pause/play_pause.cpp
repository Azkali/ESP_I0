/*
* This sample illustrates how to enter and go back to deep sleep from the
* deep sleep wake stub.
*
* Consider the use case of counting pulses from an external sensor,
* where the pulses arrive at a relatively slow rate.
*
* ESP32 is configured to go into deep sleep mode, and wake up from
* a GPIO pin connected to the external pulse source.
* Once the pulse arrives, ESP32 wakes up from deep sleep and runs
* deep sleep wake stub. This stub function is stored in RTC fast
* memory, so it can run without waiting for the whole firmware
* to be loaded from flash.
*
* This function (called wake_stub below) increments the pulse counter,
* stored in RTC_SLOW_MEM. This memory is also preserved when going
* into deep sleep. Then the wake stub decides whether to continue
* booting the firmware, or to go back to sleep. In this simple example,
* the stub starts firmware when the pulse counter reaches 100.
* Note: in real application, counting needs to be continued when the
* application has started, for example using the PCNT peripheral.
*
*/

#include <stdio.h>
#include <string.h>
#include "esp_sleep.h"
#include "esp_attr.h"
#include "rom/rtc.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/uart_reg.h"
#include "soc/timer_group_reg.h"
#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include "rom/ets_sys.h"

#include "play_pause.h"
#include "bt_app_av.h"
#include "esp_log.h"


BtPlayPauseHandler::BtPlayPauseHandler(gpio_num_t btnNum)
: BtCmdHandler<PlayPauseConfig, BtPlayPauseHandler>(generateConfig(btnNum)) {
}

void BtPlayPauseHandler::handleEvent() {
	ESP_LOGI("tag", "Hello");
}

PlayPauseConfig BtPlayPauseHandler::generateConfig(gpio_num_t btnNum){
	PlayPauseConfig config = {
		btnNum
	};
	return config;
}


extern "C" {

 BtPlayPauseHandler *ppHandler = NULL;

void play_pause_gpio_config(gpio_num_t pulseButtonNum) {
	ppHandler = new BtPlayPauseHandler(pulseButtonNum);
	// Set the value
	/*
	gpio_config.pulse_button_num = pulseButtonNum;

	gpio_set_direction((gpio_num_t)pulseButtonNum, GPIO_MODE_INPUT);
	gpio_set_pull_mode((gpio_num_t)pulseButtonNum, GPIO_PULLUP_ONLY);
	gpio_set_intr_type((gpio_num_t)pulseButtonNum, GPIO_INTR_NEGEDGE);

	printf("GPIO NUM: %d\n", pulseButtonNum);
	gpio_isr_handler_add(
		(gpio_num_t)pulseButtonNum,
		(gpio_isr_t)esp_avrc_ct_send_passthrough_cmd(0, ESP_AVRC_PT_CMD_PAUSE, 0),
		&gpio_config
	);
*/	
}
}