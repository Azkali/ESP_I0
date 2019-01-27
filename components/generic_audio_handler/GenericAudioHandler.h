#include <string>;

#define FLAC "flac"
#define MP3 "mp3"
#define AAC "aac"

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

	// Properties
	void *decoderConfig;
	EAudioTrackType trackType;
	string currentTrackPath;
	// Pipeline & audio properties
	audio_pipeline_handle_t pipeline;
	audio_element_handle_t fatfsStreamReader;
	audio_element_handle_t i2sStreamWriter;
	audio_element_handle_t audioDecoder;
};