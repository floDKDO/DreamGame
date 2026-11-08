#include "buffer.h"

namespace al
{

//si path désigne un son (dossier sounds/), celui-ci doit être en mono pour bénéficier de l'atténuation
//=> documentation OpenAL : "Buffers containing more than one channel of data will be played without 3D spatialization."
Buffer::Buffer(std::string_view path)
	: wav_buffer_(path), size_(1)
{
	alGenBuffers(size_, &id_);
	alBufferData(id_, wav_buffer_.get_format(), wav_buffer_.get_audio_data(), wav_buffer_.get_length(), wav_buffer_.get_sample_rate());
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
}

}