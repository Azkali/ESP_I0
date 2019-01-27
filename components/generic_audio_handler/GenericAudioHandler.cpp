#include "./GenericAudioHandler.h";

static const char *TAG = "GenericAudioHandler";

GenericAudioHandler *GenericAudioHandler::instance = nullptr;

GenericAudioHandler *GenericAudioHandler::getInstance(){
	if(GenericAudioHandler::instance == nullptr){
		GenericAudioHandler::instance = new GenericAudioHandler();
	}
	return GenericAudioHandler::instance;
}

EAudioTrackType GenericAudioHandler::getTrackType(){
	// If we did not set a track path, then the track type has no meaning
	if(this->currentTrackPath == nullptr){
		throw new exception("No track set.");
	}
	return this->trackType;
}

bool GenericAudioHandler::setTrack(String trackPath){
	size_t lastDotPos = trackPath->find_last_of('.');
	if(lastDotPos == string::npos){
		throw new exception("Could not get the extension");
	}
	string extension = trackPath->substr(lastDotPos + 1, string::npos)->toLower();
	
	// Find the format
	EAudioTrackType newTrackType;
	if(extension == MP3){
		newTrackType = EAudioTrackType::MP3;
	} else if(extension == AAC){
		newTrackType = EAudioTrackType::AAC;
	} else if(extension == FLAC){
		newTrackType = EAudioTrackType::FLAC;
	} else {
		throw new exception("The extension \"" + extension + "\" is not recognized.");
	}


	if(this->currentTrackPath != nullptr && newTrackType != this->trackType){
		this->releaseTrackResources();

		// Switch on track type to initialize the correct decoder
		switch(this->trackType){
			case EAudioTrackType::FLAC:{
				this->allocateFlacResources();
			} break;

			case EAudioTrackType::MP3:{
				this->allocateMp3Resources();
			} break;

			case EAudioTrackType::AAC:{
				this->allocateAacResources();
			} break;
			
			// ...
		}
	}

	this->trackType = newTrackType;
	this->currentTrackPath = trackPath;

	ESP_LOGI(TAG, "[3.6] Setup uri (file as fatfs_stream, adequate decoder, and default output is i2s)");
	audio_element_set_uri(this->fatfsStreamReader, this->currentTrackPath);


	return true;
}

void GenericAudioHandler::allocateFlacResources{
	ESP_LOGI(TAG, "[3.3] Create flac decoder to decode flac file");
	this->decoderConfig = new flac_decoder_cfg_t();
	*(this->decoderConfig) = DEFAULT_FLAC_DECODER_CONFIG();
	this->audioDecoder = flac_decoder_init(this->decoderConfig);

	audio_pipeline_register(this->pipeline, this->audioDecoder, FLAC);
	ESP_LOGI(TAG, "[3.5] Link it together [sdcard]-->fatfs_stream-->flac_decoder-->i2s_stream-->[codec_chip]");
	audio_pipeline_link(this->pipeline, (const char *[]) {"file", FLAC, "i2s"}, 3);
}

void GenericAudioHandler::allocateAacResources{
}

void GenericAudioHandler::allocateMp3Resources{
}

bool GenericAudioHandler::play(){

}

bool GenericAudioHandler::play(string trackPath){
	this->setTrack(trackPath);
	this->play();
}


GenericAudioHandler::GenericAudioHandler(){
	this->decoderConfig = nullptr;
	this->currentTrackPath = nullptr;

	this->allocateResources();
}

void GenericAudioHandler::allocateResources(){
	ESP_LOGI(TAG, "[2.1] Create i2s stream to write data to codec chip");
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_EXTERNAL_DAC_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    this->i2sStreamWriter = i2s_stream_init(&i2s_cfg);

    ESP_LOGI(TAG, "[3.0] Create audio pipeline for playback");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    this->pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(this->pipeline);

    ESP_LOGI(TAG, "[3.1] Create fatfs stream to read data from sdcard");
    fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
    fatfs_cfg.type = AUDIO_STREAM_READER;
    this->fatfsStreamReader = fatfs_stream_init(&fatfs_cfg);



    ESP_LOGI(TAG, "[3.4] Register all elements to audio pipeline");
    audio_pipeline_register(this->pipeline, this->fatfsStreamReader, "file");
	// Registration of the audio pipeline specific for the format is done in the `setTrack` method
    //audio_pipeline_register(pipeline, flac_decoder, "flac");
    audio_pipeline_register(this->pipeline, this->i2sStreamWriter, "i2s");
}

void GenericAudioHandler::releaseResources(){
	this->releaseTrackResources();

    ESP_LOGI(TAG, "[ 7 ] Stop audio_pipeline");
    audio_pipeline_terminate(this->pipeline);

    audio_pipeline_unregister(this->pipeline, this->fatfsStreamReader);
    audio_pipeline_unregister(this->pipeline, this->i2sStreamWriter);

    /* Terminal the pipeline before removing the listener */
    audio_pipeline_remove_listener(this->pipeline);

    /* Stop all periph before removing the listener */
    esp_periph_stop_all();
    audio_event_iface_remove_listener(esp_periph_get_event_iface(), evt);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(this->pipeline);
    audio_element_deinit(this->fatfsStreamReader);
    audio_element_deinit(this->i2sStreamWriter);
    esp_periph_destroy();
}

void GenericAudioHandler::releaseTrackResources(){
	delete this->decoderConfig;
	this->currentTrackPath = nullptr;
    audio_pipeline_unregister(this->pipeline, this->audioDecoder);
    audio_element_deinit(this->audioDecoder);
}