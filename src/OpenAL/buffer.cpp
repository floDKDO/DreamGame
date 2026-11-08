#include "buffer.h"
#include "Logging/logging.h"

#include <string>

namespace al
{

Buffer::Buffer(std::string_view path)
	: wav_buffer_(path), size_(1)
{
	alGenBuffers(size_, &id_);
	check_error("alGenBuffers()");

	alBufferData(id_, wav_buffer_.get_format(), wav_buffer_.get_audio_data(), wav_buffer_.get_length(), wav_buffer_.get_sample_rate());
	check_error("alBufferData()");
}

Buffer::Buffer(Buffer&& buffer)
	: wav_buffer_(std::move(buffer.wav_buffer_)), id_(buffer.id_), size_(buffer.size_)
{
	buffer.size_ = 0;
}

Buffer& Buffer::operator=(Buffer&& buffer)
{
	if(this == &buffer)
	{
		return *this;
	}

	destroy();

	wav_buffer_ = std::move(buffer.wav_buffer_);
	id_ = buffer.id_;
	size_ = buffer.size_;
	buffer.size_ = 0;
	return *this;
}

Buffer::~Buffer()
{
	destroy();
	size_ = 0;
}

ALuint Buffer::get_id() const
{
	return id_;
}

void Buffer::destroy() const
{
	alDeleteBuffers(size_, &id_);
	check_error("alDeleteBuffers()");
}

void check_error(std::string_view function_name)
{
	if(ALenum al_error = alGetError(); al_error != AL_NO_ERROR)
	{
		logging::log("Error in " + std::string(function_name), logging::Severity::WARNING);
	}
}

}