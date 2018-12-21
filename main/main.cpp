// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "driver/i2s.h"
#include "driver/gpio.h"

//#include "deep_sleep.h"
//#include "play_pause.h"

#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_avrc_api.h"

/* added from ssd1331_test.cpp */
#include "gui_handler.h"

#include "sdkconfig.h"

static char tag[] = "SSD1331_test";

#if defined USE_ADAFRUIT_LIB
#define PIN_NUM_MOSI CONFIG_HW_LCD_MOSI_GPIO
#define PIN_NUM_CLK  CONFIG_HW_LCD_CLK_GPIO
#define PIN_NUM_CS   CONFIG_HW_LCD_CS_GPIO
#define PIN_NUM_DC   CONFIG_HW_LCD_DC_GPIO
#define PIN_NUM_RST  CONFIG_HW_LCD_RESET_GPIO

	DISP_DESC disp = DISP_DESC(PIN_NUM_CS, PIN_NUM_DC, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_RST);
#else
	DISP_DESC disp = DISP_DESC();
#endif

void ssd1331_test(void *ignore) {
	ESP_LOGD(tag, ">> entry point ssd1331_final");
	// Initialize display
	disp.begin();
	disp.clearScreen();
	disp.setTextWrap(0);
	disp.setRotation(2);

	gui.setDisplay(&disp);
	gui.Logo();
	vTaskDelay(1000);
	disp.clearScreen();
	disp.clearScreen();
	
	gui.welcomeScreen();
	vTaskDelay(1000);
	disp.clearScreen();
	disp.clearScreen();
	gui.Logo();
	vTaskDelay(100);

	ESP_LOGD(tag, "<< exit point ssd1331_final");
	vTaskDelete(NULL);
}

extern "C"{

	extern void ap_mode_start();
	extern void bt_sink(void);

	/* event handler for OLED screen */
	esp_err_t event_handler(void *ctx, system_event_t *event)
	{
		return ESP_OK;
	}

	void app_main() {
		

		/* OLED Launch*/
		ESP_LOGI("tag", ">> app_main");
		xTaskCreatePinnedToCore(&ssd1331_test, "ssd1331_final", 8048, NULL, 5, NULL, 0);
		ESP_LOGI("tag", ">> app_main");
		
		bt_sink();

		ap_mode_start();

		/* Deep-sleep Function */
		//ds_gpio_config(PULSE_CNT_GPIO_NUM_33);

		/* Play/Pause function */
		// play_pause_gpio_config(PULSE_CNT_GPIO_NUM_12);

	}

}