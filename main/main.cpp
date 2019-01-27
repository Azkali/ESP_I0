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

#include "Arduino.h"

#include <string>
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
/*#include "bt_app_core.h"
#include "bt_app_av.h"*/
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


//TFT_eSPI
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#define TFT_GREY 0x5AEB // New colour

//ui-GUI
/*#include "gui_handler.h"

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
	TFT_eSPI disp = TFT_eSPI();
#endif*/

//TFT_eSPI
TFT_eSPI tft = TFT_eSPI();  // Invoke library

void launch_ui(void *arg) {
	tft.init();
	tft.setRotation(0);
	// Fill screen with grey so we can see the effect of printing with and without 
	// a background colour defined
	tft.fillScreen(TFT_BLACK);

	// Set "cursor" at top left corner of display (0,0) and select font 2
	// (cursor will move to next line automatically during printing with 'tft.println'
	//  or stay on the line is there is room for the text with tft.print)
	tft.setCursor(0, 0, 2);

	tft.println("Hey");

	vTaskDelete(NULL);
}

/*void ssd1331_test(void *ignore) {
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
}*/

extern "C"{

	extern void ap_mode_start(void *arg);
	extern void bt_sink(void *arg);
	void app_main();
}
	/* event handler for OLED screen */
	esp_err_t event_handler(void *ctx, system_event_t *event)
	{
		return ESP_OK;
	}

	void app_main(/*void* arg*/) {
		xTaskCreatePinnedToCore(&bt_sink, "bt-sink", 8192, NULL, 3, NULL, 0); 

		/* OLED Launch*/
		ESP_LOGI("tag", ">> app_main");
		xTaskCreatePinnedToCore(&launch_ui, "ssd1331_final", 8192, NULL, 3, NULL, 1);
		ESP_LOGI("tag", ">> app_main");
		initArduino();
		
		//xTaskCreatePinnedToCore(&ap_mode_start, "ap-mode", 8192, NULL, 4, NULL, 0);

		/* Deep-sleep Function */
		//ds_gpio_config(PULSE_CNT_GPIO_NUM_33);

		/* Play/Pause function */
		// play_pause_gpio_config(PULSE_CNT_GPIO_NUM_12);
	}