#pragma once

#include "RAII_SDL3/wav_buffer.h"

#include <AL/al.h>
#include <string_view>

namespace audio
{

class Buffer
{
	public:
		explicit Buffer(std::string_view path);

		ALuint get_id() const;
		void destroy() const;

	private:
		sdl::WAVBuffer wav_buffer_;
		ALuint id_;
};

}