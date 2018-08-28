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
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/uart_reg.h"
#include "soc/timer_group_reg.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "rom/ets_sys.h"

#include "deep_sleep.h"

//extern "C" {
// Pulse counter value, stored in RTC_SLOW_MEM
static size_t RTC_DATA_ATTR s_pulse_count;
static size_t RTC_DATA_ATTR s_max_pulse_count;

// Function which runs after exit from deep sleep
static void RTC_IRAM_ATTR wake_stub();

/* static void deep_sleep(void *arg)
{
    deepsleep_config *config = (deepsleep_config *)arg;

    //printf("Pulse button identifier: %d\n", config->pulse_button_num);

    s_pulse_count = 0;
    s_max_pulse_count = 2;
    vTaskDelay(1000/portTICK_PERIOD_MS);

    // Set the wake stub function
    esp_set_deep_sleep_wake_stub(&wake_stub);

    // Wake up on low logic level
    ESP_ERROR_CHECK( esp_sleep_enable_ext1_wakeup(
            1LL << config->pulse_button_num, ESP_EXT1_WAKEUP_ALL_LOW) );

    // Enter deep sleep
    esp_deep_sleep_start();
}*/

static const char RTC_RODATA_ATTR wake_fmt_str[] = "Sleeping... Good Night.. Zzzz!\nClick to wake up\n"; //= %d\n";
static const char RTC_RODATA_ATTR sleep_fmt_str[] = "Waking Up!\n";

static void RTC_IRAM_ATTR wake_stub()
{
    // Increment the pulse counter
    s_pulse_count++;
    // and print the pulse counter value:
    ets_printf(wake_fmt_str, s_pulse_count);

    if (s_pulse_count >= s_max_pulse_count) {
        // On revision 0 of ESP32, this function must be called:
        esp_default_wake_deep_sleep();

        // Return from the wake stub function to continue
        // booting the firmware.
        return;
    }
    // Pulse count is <s_max_pulse_count, go back to sleep
    // and wait for more pulses.

    // Wait for pin level to be high.
    // If we go to sleep when the pin is still low, the chip
    // will wake up again immediately. Hardware doesn't have
    // edge trigger support for deep sleep wakeup.
    do {
        while (PULSE_CNT_IS_LOW()) {
            // feed the watchdog
            REG_WRITE(TIMG_WDTFEED_REG(0), 1);
        }
        // debounce, 10ms
        ets_delay_us(10000);
    } while (PULSE_CNT_IS_LOW());

    // Print status
    ets_printf(sleep_fmt_str);
    // Wait for UART to end transmitting.
    while (REG_GET_FIELD(UART_STATUS_REG(0), UART_ST_UTX_OUT)) {
        ;
    }
    // Set the pointer of the wake stub function.
    REG_WRITE(RTC_ENTRY_ADDR_REG, (uint32_t)&wake_stub);
    // Go to sleep.
    CLEAR_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
    SET_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
    // A few CPU cycles may be necessary for the sleep to start...
    while (true) {
        ;
    }
    // never reaches here.
}






DeepSleepHandler::DeepSleepHandler(gpio_num_t pulseButtonNum)
: SimpleBtnCmdHandler(pulseButtonNum) {
}

void DeepSleepHandler::handleEvent() {
	ESP_LOGI("tag", "Btn Deep Sleep triggered");
}

 static DeepSleepHandler *dsHandler = NULL;
 
 void ds_gpio_config(gpio_num_t pulseButtonNum) {
    dsHandler = new DeepSleepHandler(pulseButtonNum);
   	dsHandler->buttonBind();
    // Allocate a static variable
   /*static deepsleep_config gpio_config = {.pulse_button_num = 0};
   // Set the value
   gpio_config.pulse_button_num = pulse_button_num;

   gpio_set_direction((gpio_num_t)pulseButtonNum, GPIO_MODE_INPUT);
   gpio_set_pull_mode((gpio_num_t)pulseButtonNum, GPIO_PULLUP_ONLY);
   gpio_set_intr_type((gpio_num_t)pulseButtonNum, GPIO_INTR_NEGEDGE);
 
   printf("GPIO NUM: %d\n", pulseButtonNum);
   
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(
    (gpio_num_t)pulse_button_num,
    &deep_sleep,
    &gpio_config
  );*/
}
//}
/*
 ESP32CPP::GPIO dsHandler;
 
 void ds_gpio_config(gpio_num_t ButtonNum) {
     
    dsHandler.setInput(ButtonNum);
    gpio_set_pull_mode((gpio_num_t)ButtonNum, GPIO_PULLUP_ONLY);
    dsHandler.setInterruptType(ButtonNum, GPIO_INTR_NEGEDGE);

    printf("GPIO NUM: %d\n", ButtonNum);
    dsHandler.addISRHandler(ButtonNum, (gpio_isr_t)&deep_sleep, NULL);
 }*/