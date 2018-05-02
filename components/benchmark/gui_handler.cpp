/*
 * gui_handler.cpp
 *
 *  Created on: 11.06.2017
 *      Author: chegewara
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include "gui_handler.h"

#include "bt_app_av.h"
#include "esp_avrc_api.h"



GuiHandler::GuiHandler(DISP_DESC *disp) {
	display = disp;
	x0 = 25; //display->width()/2;

	if(disp != NULL){
		y0 = display->height()/2;
	}
}

void GuiHandler::setDisplay(DISP_DESC *disp){
	display = disp;
	y0 = display->height()/2;
}
/*
void GuiHandler::testText() {

	display->setCursor(0, 0);
	display->setTextColor(WHITE);
	display->setTextSize(1);
	println((char*) "Hello World!");
	display->setCursor(0, display->getCursorY() + 8);
	display->setTextColor(YELLOW);
	display->setTextSize(2);
	println((char*) "1234.56");
	display->setCursor(0, display->getCursorY() + 15);
	display->setTextColor(RED);
	display->setTextSize(3);
	println((char*) "ESP32");
	display->setCursor(0, display->getCursorY() + 23);
	display->setTextSize(1);
	display->setTextColor(BLUE);
	println((char*) "FULL COLOR!");
	println((char*) "");
}*/

void GuiHandler::welcomeScreen() {
	display->setCursor(0, 0);
	display->setTextColor(WHITE);
	display->setTextSize(1);
	println((char*) "Wello Horld!");
	display->setCursor(0, display->getCursorY() + 8);
	display->setTextColor(YELLOW);
	display->setTextSize(1);
	println((char*) "Listen to your");
	display->setCursor(0, display->getCursorY() + 9);
	display->setTextColor(YELLOW);
	display->setTextSize(1);
	println((char*) "library anywhere");
	display->setCursor(0, display->getCursorY() + 10);
	display->setTextColor(YELLOW);
	display->setTextSize(1);
	println((char*) "without wires!");
	display->setCursor(0, display->getCursorY() + 11);
	display->setTextColor(RED);
	display->setTextSize(1);
	println((char*) "ESP_I0");
	display->setCursor(0, display->getCursorY() + 13);
	display->setTextSize(1);
	display->setTextColor(BLUE);
	println((char*) "In FULL COLOR!");
	println((char*) "");	
}

GuiHandler *GuiHandler::setTitle(string _title) {
	this->title = new string(_title);
	return this;
}
GuiHandler *GuiHandler::setArtist(string _artist) {
	this->artist = new string(_artist);
	return this;
}
GuiHandler *GuiHandler::setAlbum(string _album) {
	this->album = new string(_album);
	return this;
}

GuiHandler *GuiHandler::refreshTitle(){
	//scrollLine(title);
	this->refreshRow(POS_TITLE, title, RED);
	//scrollLine(title);
	return this;
}

GuiHandler *GuiHandler::refreshArtist(){
	//scrollLine(artist);
	this->refreshRow(POS_ARTIST, artist, YELLOW);	
	//scrollLine(artist);
	return this;
}

GuiHandler *GuiHandler::refreshAlbum(){
	//scrollLine(album);
	this->refreshRow(POS_ALBUM, album, CYAN);
	//scrollLine(album);
	return this;
}


GuiHandler *GuiHandler::scrollLine(string *line) {
	display->ScrollSet(12,0,POS_ALBUM,0,20);
	display->scroll(line->c_str());
	return this;
}	

GuiHandler *GuiHandler::refreshScreen(){
	refreshTitle();
	refreshArtist();
	return refreshAlbum();
	 
}

/*void GuiHandler::scrollLineSec(string *line) {
	int16_t x1,y1,x2,y2,width;
	display->getTextBounds(line, 0, 0, &x1, &y1, &x2, &y2);
	width=x2-x1;
}*/

void GuiHandler::refreshRow(int16_t posx, string *datdata, color_t color){
	display->clearWindow(0, posx, display->width(), posx + INTERLINE);
	display->setCursor(0, posx + 2);
	display->setTextColor(color);
	display->setTextSize(1);
	display->setTextWrap(false);
	char *newStr = strdup(datdata->c_str());
	//drawString(newStr);
	println(newStr);
	printf(newStr);
	free(newStr);
}

void GuiHandler::drawString(const char *string) {
    if(string!=NULL) {
		while(*string)
			display->write(*string++);
    }
}

void GuiHandler::println (char* text){
	while(*text != 0) {
		display->write(*text);
		text++;
	}
}


/*
void GuiHandler::testFastLines() {

}

void GuiHandler::testLines() {
	int           x1, y1, x2, y2,
				w = display->width(),
				h = display->height();

	display->clearScreen();
	uint16_t color = RED;

	x1 = y1 = 0;
	y2    = h - 1;
	for (x2 = 0; x2 < w; x2 += 6) display->drawLine(x1, y1, x2, y2, color);
	x2    = w - 1;
	for (y2 = 0; y2 < h; y2 += 6) display->drawLine(x1, y1, x2, y2, color);

	vTaskDelay(500);
	display->clearScreen();

	x1    = w - 1;
	y1    = 0;
	y2    = h - 1;
	for (x2 = 0; x2 < w; x2 += 6) display->drawLine(x1, y1, x2, y2, color);
	x2    = 0;
	for (y2 = 0; y2 < h; y2 += 6) display->drawLine(x1, y1, x2, y2, color);

	vTaskDelay(500);
	display->clearScreen();

	x1    = 0;
	y1    = h - 1;
	y2    = 0;
	for (x2 = 0; x2 < w; x2 += 6) display->drawLine(x1, y1, x2, y2, color);
	x2    = w - 1;
	for (y2 = 0; y2 < h; y2 += 6) display->drawLine(x1, y1, x2, y2, color);

	vTaskDelay(500);
	display->clearScreen();

	x1    = w - 1;
	y1    = h - 1;
	y2    = 0;
	for (x2 = 0; x2 < w; x2 += 6) display->drawLine(x1, y1, x2, y2, color);
	x2    = 0;
	for (y2 = 0; y2 < h; y2 += 6) display->drawLine(x1, y1, x2, y2, color);


}

void GuiHandler::testRectangles(int alpha) {
	uint16_t x, y, x1, y1, x2, y2, x3, y3, r=25;

	x = x0 + sin(alpha*0.0174532925) * r;
	y = y0 - cos(alpha*0.0174532925) * r;

	alpha += 90;
	x1 = x0 + sin(alpha*0.0174532925) * r;
	y1 = y0 - cos(alpha*0.0174532925) * r;

	alpha += 90;
	x2 = x0 + sin(alpha*0.0174532925) * r;
	y2 = y0 - cos(alpha*0.0174532925) * r;

	alpha += 90;
	x3 = x0 + sin(alpha*0.0174532925) * r;
	y3 = y0 - cos(alpha*0.0174532925) * r;

	display->drawLine(x, y, x1, y1, GREEN);
	display->drawLine(x1, y1, x2, y2, GREEN);
	display->drawLine(x2, y2, x3, y3, GREEN);
	display->drawLine(x3, y3, x, y, GREEN);
}

void GuiHandler::testRectangles2(int alpha) {
	uint16_t x, y, x1, y1, x2, y2, x3, y3, r=20;

	x = x0 + sin(alpha*0.0174532925) * r;
	y = y0 - cos(alpha*0.0174532925) * r;

	x1 = x0 + sin(alpha*0.0174532925) * r;
	y1 = y0 + cos(alpha*0.0174532925) * r;

	x2 = x0 - sin(alpha*0.0174532925) * r;
	y2 = y0 + cos(alpha*0.0174532925) * r;

	x3 = x0 - sin(alpha*0.0174532925) * r;
	y3 = y0 - cos(alpha*0.0174532925) * r;

	display->drawLine(x, y, x1, y1, BLUE);
	display->drawLine(x1, y1, x2, y2, BLUE);
	display->drawLine(x2, y2, x3, y3, BLUE);
	display->drawLine(x3, y3, x, y, BLUE);
}

void GuiHandler::testBox3d() {
	uint16_t x, y, r=20, alpha=45;

	x = x0-r + sin(alpha*0.0174532925) * r;
	y = y0-r+10 - cos(alpha*0.0174532925) * r;

	display->fillRect(x0-r, y0-r+10, 2*r, 2*r, RED);
	for(int i=0; i<r*2;i++){
		display->drawLine(x0-r+i, y0-r+10, x+i, y, BLUE);
		display->drawLine(x0+r, y0-r+i+10, x+2*r, y+i, GREEN);
	}

}*/
