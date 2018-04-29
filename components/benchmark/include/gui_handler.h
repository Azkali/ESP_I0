/*
 * GuiHandler.h
 *
 *  Created on: 11.06.2017
 *      Author: chegewara
 */
#ifndef _GUI_HANDLER_
#define _GUI_HANDLER_

#define USE_ADAFRUIT_LIB

#include <string>
#include <string.h>


using namespace std;

#if defined USE_ADAFRUIT_LIB
	#include "Adafruit_SSD1331.h"
	#define DISP_DESC Adafruit_SSD1331
#else
	#include "SSD1331.h"
	#define DISP_DESC SSD1331
#endif

#define INTERLINE 12
#define POS_TITLE INTERLINE
#define POS_ARTIST POS_TITLE + INTERLINE
#define POS_ALBUM POS_ARTIST + INTERLINE
#define POS_GENRE POS_ALBUM + INTERLINE

typedef uint16_t color_t;

class GuiHandler {

public:
	GuiHandler(DISP_DESC *disp = NULL);
	
	void setDisplay(DISP_DESC *disp);
	void testLines();
	void testFastLines();
	void drawClock();
	void testText();
	void testRectangles(int alpha);
	void testRectangles2(int alpha);
	void testBox3d();
	void welcomeScreen();
	void musicScreen();
	GuiHandler *refreshTitle();
	GuiHandler *refreshArtist();
	GuiHandler *refreshAlbum();
	GuiHandler *refreshGenre();
	GuiHandler *refreshScreen();
	GuiHandler *setTitle(string _title);
	GuiHandler *setArtist(string _artist);
	GuiHandler *setAlbum(string _album);
	GuiHandler *setGenre(string _genre);


private:
	void refreshRow(int16_t posx, string *datdata, color_t color);
	void println(char*);
	
	DISP_DESC *display;
	uint16_t x0, y0;
	string *title = NULL;
	string *artist = NULL;
	string *album = NULL;
	string *genre = NULL;
};

extern GuiHandler gui;

#endif