/*
 * GuiHandler.h
 *
 *  Created on: 11.06.2017
 *      Author: chegewara
 */
#ifndef _GUI_HANDLER_
#define _GUI_HANDLER_

//#define USE_ADAFRUIT_LIB

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include "esp_log.h"
#include "bluetooth_service.h"
#include <math.h>

using namespace std;

#if defined USE_ADAFRUIT_LIB
	#include "Adafruit_SSD1331.h"
	#define DISP_DESC Adafruit_SSD1331
#else
	#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
	#include <SPI.h>	
	#define DISP_DESC TFT_eSPI
#endif

#define INTERLINE 12
#define POS_TITLE INTERLINE
#define POS_ARTIST POS_TITLE + INTERLINE
#define POS_ALBUM POS_ARTIST + INTERLINE

typedef uint16_t color_t;

class GuiHandler {

public:
	GuiHandler(DISP_DESC *disp = NULL);

	void welcomeScreen();
	void musicScreen();
	void Logo();
	void setDisplay(DISP_DESC *disp);
	GuiHandler *refreshTitle();
	GuiHandler *refreshArtist();
	GuiHandler *refreshAlbum();
	GuiHandler *refreshScreen();
	GuiHandler *setTitle(String _title);
	GuiHandler *setArtist(String _artist);
	GuiHandler *setAlbum(String _album);
	// GuiHandler *scrollLine(String *line);

private:
	void refreshRow(int16_t posx, String *datdata, color_t color);
	// void println(char *text);
	
	DISP_DESC *display;
	uint16_t x0, y0;
	String *title = NULL;
	String *artist = NULL;
	String *album = NULL;
	String *genre = NULL;
};

extern GuiHandler gui;

#endif