#pragma once

#include <SDL3/SDL.h>
#include <string_view>

namespace sdl
{

class WAVBuffer
{
	public:
		WAVBuffer(std::string_view path);
		WAVBuffer(const WAVBuffer& wav_buffer) = delete;
		WAVBuffer(WAVBuffer&& wav_buffer) = delete;
		WAVBuffer& operator=(const WAVBuffer& wav_buffer) = delete;
		WAVBuffer& operator=(WAVBuffer&& wav_buffer) = delete;
		~WAVBuffer();

		const Uint8* const get_audio_data() const;
		int get_format() const;
		int get_number_of_channels() const;
		int get_sample_rate() const;
		Uint32 get_length() const;

	private:
		Uint8* audio_data_;
		SDL_AudioSpec audio_spec_;
		Uint32 audio_len_;
};

}