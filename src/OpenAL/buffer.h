#pragma once

#include "RAII_SDL3/wav_buffer.h"

#include <AL/al.h>
#include <string_view>

namespace al
{

class Buffer
{
	public:
		explicit Buffer(std::string_view path);
		Buffer(const Buffer&) = delete;
		Buffer(Buffer&& buffer);
		Buffer& operator=(const Buffer&) = delete;
		Buffer& operator=(Buffer&& buffer);
		~Buffer();

		ALuint get_id() const;
		void destroy() const;

	private:
		sdl::WAVBuffer wav_buffer_;
		ALuint id_;
		ALsizei size_;
};

void check_error(std::string_view function_name);

}