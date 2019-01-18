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

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library

void launch_ui(void *arg) {
	tft.init();
	tft.setRotation(3);

	// Fill screen with grey so we can see the effect of printing with and without 
	// a background colour defined
	tft.fillScreen(TFT_GREY);

	// Set "cursor" at top left corner of display (0,0) and select font 2
	// (cursor will move to next line automatically during printing with 'tft.println'
	//  or stay on the line is there is room for the text with tft.print)
	tft.setCursor(0, 0, 2);
	// Set the font colour to be white with a black background, set text size multiplier to 1
	tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
	// We can now plot text on screen using the "print" class
	tft.println("Hello World!");

	// Set the font colour to be yellow with no background, set to font 7
	tft.setTextColor(TFT_YELLOW); tft.setTextFont(7);
	tft.println(1234.56);

	// Set the font colour to be red with black background, set to font 4
	tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
	//tft.println(3735928559L, HEX); // Should print DEADBEEF

	// Set the font colour to be green with black background, set to font 4
	tft.setTextColor(TFT_GREEN,TFT_BLACK);
	tft.setTextFont(4);
	tft.println("Groop");
	tft.println("I implore thee,");

	// Change to font 2
	tft.setTextFont(2);
	tft.println("my foonting turlingdromes.");
	tft.println("And hooptiously drangle me");
	tft.println("with crinkly bindlewurdles,");
	// This next line is deliberately made too long for the display width to test
	// automatic text wrapping onto the next line
	tft.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");

	// Test some print formatting functions
	float fnumber = 123.45;
	// Set the font colour to be blue with no background, set to font 4
	tft.setTextColor(TFT_BLUE);    tft.setTextFont(4);
	tft.print("Float = "); tft.println(fnumber);           // Print floating point number
	tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
	tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
	//delay(10000);
	vTaskDelete(NULL);
}

extern "C"{

	extern void ap_mode_start(void *arg);
	extern void bt_sink(void *arg);

	/* event handler for OLED screen */
	esp_err_t event_handler(void *ctx, system_event_t *event)
	{
		return ESP_OK;
	}

	void app_main(/*void* arg*/) {
		
		/* OLED Launch*/
		ESP_LOGI("tag", ">> app_main");
		xTaskCreatePinnedToCore(&launch_ui, "ssd1331_final", 8192, NULL, 3, NULL, 1);
		ESP_LOGI("tag", ">> app_main");

		xTaskCreatePinnedToCore(&bt_sink, "bt-sink", 8192, NULL, 3, NULL, 0); 

		//xTaskCreatePinnedToCore(&ap_mode_start, "ap-mode", 8192, NULL, 4, NULL, 0);

		/* Deep-sleep Function */
		//ds_gpio_config(PULSE_CNT_GPIO_NUM_33);

		/* Play/Pause function */
		// play_pause_gpio_config(PULSE_CNT_GPIO_NUM_12);

	}

}