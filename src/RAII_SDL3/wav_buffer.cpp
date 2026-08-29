#include "RAII_SDL3/wav_buffer.h"

#include <AL/al.h>
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

WAVBuffer::~WAVBuffer()
{
	SDL_free(audio_data_);
}

const Uint8* const WAVBuffer::get_audio_data() const
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
			if(audio_spec_.channels >= 1)
			{
				format = AL_FORMAT_MONO8;
			}
			else
			{
				format = AL_FORMAT_STEREO8;
			}
			break;

		case SDL_AUDIO_S16LE:
		case SDL_AUDIO_S16BE:
			if(audio_spec_.channels >= 1)
			{
				format = AL_FORMAT_MONO16;
			}
			else
			{
				format = AL_FORMAT_STEREO16;
			}
			break;

		case SDL_AUDIO_S32LE:
		case SDL_AUDIO_S32BE:
		case SDL_AUDIO_F32LE:
		case SDL_AUDIO_F32BE:
			if(audio_spec_.channels >= 1)
			{
				format = 0;
			}
			else
			{
				format = 0;
			}
			std::cout << "Warning: Unhandled audio format!\n";
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