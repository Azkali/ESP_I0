#include <string>
#include <map>
#include <exception>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo> 
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "esp_peripherals.h"
#include "periph_button.h"


#include "esp_peripherals.h"
#include "periph_sdcard.h"

#include "bluetooth_service.h"

using namespace std;

#define LYRAT_TOUCH_SET     GPIO_NUM_37//TOUCH_PAD_NUM9
#define LYRAT_TOUCH_PLAY    -1//TOUCH_PAD_NUM8
#define LYRAT_TOUCH_VOLUP   GPIO_NUM_38//TOUCH_PAD_NUM7
#define LYRAT_TOUCH_VOLDWN  GPIO_NUM_39//TOUCH_PAD_NUM4

enum EAudioTrackType{
	MP3,
	FLAC,
	AAC
};

class GenericAudioHandler
{
public:
	// Singleton defs
	GenericAudioHandler *getInstance();

	EAudioTrackType getTrackType();
	bool setTrack(string trackPath);
	bool play();
	bool play(string trackPath);
	
	// No destructor, singleton
	//~GenericAudioHandler();
private:
	// Singleton defs
	GenericAudioHandler();
	static GenericAudioHandler* instance;

	// Pipeline components def/undef methods
	void allocateResources();
	void allocateFlacResources();
	void allocateMp3Resources();
	void allocateAacResources();
	void releaseResources();
	void releaseTrackResources();
	void eventListener();
	void GpioBtSinkHandler();

	// Properties
	void *decoderConfig;
	EAudioTrackType trackType;
	string currentTrackPath;
	// Pipeline & audio properties
	audio_pipeline_handle_t pipeline;
	audio_element_handle_t fatfsStreamReader;
	audio_element_handle_t i2sStreamWriter;
	audio_element_handle_t audioDecoder;
	audio_event_iface_handle_t evt;
	audio_event_iface_msg_t msg;
	audio_element_info_t music_info;
	esp_periph_handle_t bt_periph;
	esp_periph_handle_t button_handle;
};