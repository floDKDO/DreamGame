#include "openal.h"
#include "RAII_SDL3/wav_buffer.h"

#include <AL/al.h>
#include <unordered_map>

namespace audio
{

struct SourceInfo
{
	ALuint source_;
	ALuint buffer_;
};
std::unordered_map<std::string, SourceInfo> sources_;

void set_listener_position(glm::vec3 position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void set_listener_velocity(glm::vec3 velocity)
{
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void set_listener_orientation(glm::vec3 forward, glm::vec3 up)
{
	float listener_orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, listener_orientation);
}

void create_source(const std::string& model_name, std::string_view path)
{
	sdl::WAVBuffer wav_buffer(path);
	alGenBuffers(1, &sources_[model_name].buffer_);
	alBufferData(sources_[model_name].buffer_, wav_buffer.get_format(), wav_buffer.get_audio_data(), wav_buffer.get_length(), wav_buffer.get_sample_rate());

	alGenSources(1, &sources_[model_name].source_);
	alSourcei(sources_[model_name].source_, AL_BUFFER, sources_[model_name].buffer_);

	alSource3f(sources_[model_name].source_, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(sources_[model_name].source_, AL_LOOPING, AL_FALSE);
}

void destroy_source(const std::string& model_name)
{
	alDeleteSources(1, &sources_[model_name].source_);
	alDeleteBuffers(1, &sources_[model_name].buffer_);
}

void set_source_pitch(const std::string& model_name, float value)
{
	alSourcef(sources_[model_name].source_, AL_PITCH, value);
}

void set_source_gain(const std::string& model_name, float value)
{
	alSourcef(sources_[model_name].source_, AL_GAIN, value);
}

void set_source_reference_distance(const std::string& model_name, float value)
{
	alSourcef(sources_[model_name].source_, AL_REFERENCE_DISTANCE, value);
}

void set_source_position(const std::string& model_name, glm::vec3 position)
{
	alSource3f(sources_[model_name].source_, AL_POSITION, position.x, position.y, position.z);
}

void set_source_max_distance(const std::string& model_name, float value)
{
	alSourcef(sources_[model_name].source_, AL_MAX_DISTANCE, value);
}

void set_source_rolloff_factor(const std::string& model_name, float value)
{
	alSourcef(sources_[model_name].source_, AL_ROLLOFF_FACTOR, value);
}

void set_source_looping(const std::string& model_name)
{
	alSourcei(sources_[model_name].source_, AL_LOOPING, AL_TRUE);
}

void play_source(const std::string& model_name)
{
	alSourcePlay(sources_[model_name].source_);
}

}