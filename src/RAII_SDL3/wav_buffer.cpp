#include "wav_buffer.h"
#include "Logging/logging.h"

#include <AL/al.h>
#include <SDL3/SDL_log.h>
#include <iostream>

namespace sdl
{

WAVBuffer::WAVBuffer(std::string_view path)
{
	if(!SDL_LoadWAV(path.data(), &audio_spec_, &audio_data_, &audio_len_))
	{
		SDL_Log("(SDL_LoadWAV) %s\n", SDL_GetError());
	}
}

WAVBuffer::WAVBuffer(WAVBuffer&& wav_buffer)
	: audio_data_(wav_buffer.audio_data_), audio_spec_(wav_buffer.audio_spec_), audio_len_(wav_buffer.audio_len_)
{
	wav_buffer.audio_data_ = nullptr;
}

WAVBuffer& WAVBuffer::operator=(WAVBuffer&& wav_buffer)
{
	if(this == &wav_buffer)
	{
		return *this;
	}

	destroy();
	audio_data_ = wav_buffer.audio_data_;
	audio_spec_ = wav_buffer.audio_spec_;
	audio_len_ = wav_buffer.audio_len_;
	wav_buffer.audio_data_ = nullptr;
	return *this;
}

WAVBuffer::~WAVBuffer()
{
	destroy();
}

void WAVBuffer::destroy()
{
	SDL_free(audio_data_);
	audio_data_ = nullptr;
}

const Uint8* WAVBuffer::get_audio_data() const
{
	return audio_data_;
}

int WAVBuffer::get_format() const //format de OpenAL
{
	int format = 0;

	switch(audio_spec_.format)
	{
		case SDL_AUDIO_U8:
		case SDL_AUDIO_S8:
			switch(audio_spec_.channels)
			{
				case 1:
					format = AL_FORMAT_MONO8;
					break;

				case 2:
					format = AL_FORMAT_STEREO8;
					break;

				default:
					format = 0;
					logging::log("Unhandled number of channels!", logging::Severity::WARNING);
					break;
			}
			break;

		case SDL_AUDIO_S16LE:
		case SDL_AUDIO_S16BE:
			switch(audio_spec_.channels)
			{
				case 1:
					format = AL_FORMAT_MONO16;
					break;

				case 2:
					format = AL_FORMAT_STEREO16;
					break;

				default:
					format = 0;
					logging::log("Unhandled number of channels!", logging::Severity::WARNING);
					break;
			}
			break;

		case SDL_AUDIO_S32LE:
		case SDL_AUDIO_S32BE:
		case SDL_AUDIO_F32LE:
		case SDL_AUDIO_F32BE:
			format = 0; //TODO : gérer le cas 32 bits
			logging::log("Unhandled audio format!", logging::Severity::WARNING);
			break;
	}
	return format;
}

int WAVBuffer::get_number_of_channels() const
{
	return audio_spec_.channels;
}

int WAVBuffer::get_sample_rate() const
{
	return audio_spec_.freq;
}

Uint32 WAVBuffer::get_length() const
{
	return audio_len_;
}

}