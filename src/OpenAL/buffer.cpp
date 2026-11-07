#include "buffer.h"

namespace audio
{

Buffer::Buffer(std::string_view path)
	: wav_buffer_(path)
{
	alGenBuffers(1, &id_);
	//check_error("alGenBuffers()"); //TODO

	alBufferData(id_, wav_buffer_.get_format(), wav_buffer_.get_audio_data(), wav_buffer_.get_length(), wav_buffer_.get_sample_rate());
	//check_error("alBufferData()"); //TODO
}

ALuint Buffer::get_id() const
{
	return id_;
}

void Buffer::destroy() const
{
	alDeleteBuffers(1, &id_);
	//check_error("alDeleteBuffers()");
}

}