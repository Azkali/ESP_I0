#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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
	disp.setRotation(0);

	gui.setDisplay(&disp);
	gui.welcomeScreen();
	vTaskDelay(1000);
	disp.clearScreen();
	disp.clearScreen();
	/*gui.musicScreen(NULL);*/
	/*while(1){
	}*/
	ESP_LOGD(tag, "<< exit point ssd1331_final");
	vTaskDelete(NULL);
}
