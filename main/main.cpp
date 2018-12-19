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

#include "a2dp_source_start.h"

extern "C"{

/* event handler for OLED screen */
esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

extern void ssd1331_test(void *ignore);

/* event for handler "bt_av_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

BT_MODE = SINK;

/* handler for bluetooth stack enabled events */
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);
void a2dp_sink_startup() {
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
	ESP_ERROR_CHECK(err);

    i2s_config_t i2s_config = {
#ifdef CONFIG_A2DP_SINK_OUTPUT_INTERNAL_DAC
        mode: (i2s_mode_t)I2S_MODE_DAC_BUILT_IN,
#else
        mode: (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),                                  // Only TX
#endif
        sample_rate: 44100,
        bits_per_sample: (i2s_bits_per_sample_t)16,                                              
        channel_format: (i2s_channel_fmt_t)I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        communication_format: (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        intr_alloc_flags: ESP_INTR_FLAG_LEVEL1,                                //Interrupt level 1
        dma_buf_count: 128,                                                   //6
        dma_buf_len: 64,                                                      //60
        use_apll: false,
        fixed_mclk: 0
    };


    i2s_driver_install((i2s_port_t)0, &i2s_config, 0, NULL);
#ifdef CONFIG_A2DP_SINK_OUTPUT_INTERNAL_DAC
	i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    i2s_set_pin(0, NULL);
#else
    i2s_pin_config_t pin_config = {
        bck_io_num: CONFIG_I2S_BCK_PIN,
        ws_io_num: CONFIG_I2S_LRCK_PIN,
        data_out_num: CONFIG_I2S_DATA_PIN,
        data_in_num: -1                                                       //Not used
    };

    i2s_set_pin((i2s_port_t)0, &pin_config);
#endif

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((err = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
}

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);


    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
	esp_bt_gap_set_pin(pin_type, 4, pin_code);
}

void bt_mode() {
	if ((err = esp_bt_controller_enable()) == ESP_OK ){
		if (BT_MODE == SINK){
			return SINK;
		}
		else if (BT_MODE == SRC) {
			return SRC;
		}
		else {
			ESP_LOGE("BT_MODE: Value {SRC or SINK} is not set please choose one");
			return;
		}
	}

	else {
		ESP_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
	}
}

void ap_mode_start(){
	/* Wifi-Ap */
	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
	if ((err = esp_bt_controller_enable()) != ESP_OK ) {
		wifi_event_group = xEventGroupCreate();
		start_dhcp_server();
		initialise_wifi_in_ap();
		xTaskCreate(&print_sta_info,"print_sta_info",4096,NULL,5,NULL)
	}
	else {
		ESP_LOGE("AP_MODE: Couldn't start Wifi AP Mode because Bluetooth is select.\nIf you want to use AP MOode Please diasble BT before.\n");
		return;
	}
}

void app_main()
{

	if((err = esp_bt_controller_enable()) == ESP_OK) {
		if(bt_mode() == SINK) {

			a2dp_sink_startup();
		}
		else if(bt_mode() == SRC) {
			a2dp_source_start();
		}
	}
	else {
		ap_mode_start();
	}

    /* Deep-sleep Function */
    //ds_gpio_config(PULSE_CNT_GPIO_NUM_33);

    /* Play/Pause function */
    // play_pause_gpio_config(PULSE_CNT_GPIO_NUM_12);

    /* OLED Launch*/
    ESP_LOGI("tag", ">> app_main");
    xTaskCreatePinnedToCore(&ssd1331_test, "ssd1331_final", 8048, NULL, 5, NULL, 0);
    ESP_LOGI("tag", ">> app_main");
}


static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    ESP_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event) {
    case BT_APP_EVT_STACK_UP: {
        /* set up device name */
        char *dev_name = "ESP_I/0";
        esp_bt_dev_set_device_name(dev_name);

        /* initialize A2DP sink */
        esp_a2d_register_callback(&bt_app_a2d_cb);
        esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
        esp_a2d_sink_init();

        /* initialize AVRCP controller */
        esp_avrc_ct_init();
        esp_avrc_ct_register_callback(bt_app_rc_ct_cb);
        
        
        /* set discoverable and connectable mode, wait to be connected */
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
        break;
    }
    default:
        ESP_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}
