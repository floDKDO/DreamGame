#include "source.h"
#include "buffer.h"
#include "RAII_SDL3/wav_buffer.h"
#include "Logging/logging.h"
#include "Resource/resource_manager.h"

#include <string>
#include <iostream>

namespace audio
{

//si path désigne un son (dossier sounds/), celui-ci doit être en mono pour bénéficier de l'atténuation 
//=> documentation OpenAL : "Buffers containing more than one channel of data will be played without 3D spatialization."
Source::Source(std::size_t buffer_id, bool loop)
	: buffer_id_(buffer_id)
{
	properties_.loop_ = loop;
	init();
}

Source::Source(std::size_t buffer_id, glm::vec3 position, bool loop)
	: buffer_id_(buffer_id)
{
	properties_.loop_ = loop;
	properties_.position_ = position;
	init();
}

void Source::init()
{
	alGenSources(1, &id_);
	check_error("alGenSources()");

	Buffer* buffer = resource::get_audio_buffer(buffer_id_);
	alSourcei(id_, AL_BUFFER, buffer->get_id());
	check_error("alSourcei()");

	//valeurs par défaut
	alSourcef(id_, AL_GAIN, properties_.gain_);
	check_error("alSourcef()");
	alSourcef(id_, AL_PITCH, properties_.pitch_);
	check_error("alSourcef()");
	alSourcef(id_, AL_ROLLOFF_FACTOR, properties_.rolloff_factor_);
	check_error("alSourcef()");
	alSource3f(id_, AL_VELOCITY, properties_.velocity_.x, properties_.velocity_.y, properties_.velocity_.z);
	check_error("alSource3f()");
	alSourcei(id_, AL_LOOPING, properties_.loop_ ? AL_TRUE : AL_FALSE);
	check_error("alSourcei()");
	alSourcef(id_, AL_REFERENCE_DISTANCE, properties_.reference_distance_);
	check_error("alSourcef()");
	alSourcef(id_, AL_MAX_DISTANCE, properties_.max_distance_);
	check_error("alSourcef()");
	alSource3f(id_, AL_POSITION, properties_.position_.x, properties_.position_.y, properties_.position_.z);
	check_error("alSource3f()");
}

void Source::check_error(std::string_view function_name)
{
	if(ALenum al_error = alGetError(); al_error != AL_NO_ERROR)
	{
		logging::log("Error in " + std::string(function_name), logging::Severity::WARNING);
	}
}

void Source::play() const
{
	alSourcePlay(id_);
	check_error("alSourcePlay()");
}

void Source::destroy() const
{
	alSourceStop(id_);
	check_error("alSourceStop()");

	alSourcei(id_, AL_BUFFER, NULL);
	check_error("alSourcei()");

	alDeleteSources(1, &id_);
	check_error("alDeleteSources()");
}

void Source::set_gain(float value)
{
	properties_.gain_ = value;
	alSourcef(id_, AL_GAIN, properties_.gain_);
	check_error("alSourcef()");
}

void Source::set_pitch(float value)
{
	properties_.pitch_ = value;
	alSourcef(id_, AL_PITCH, properties_.pitch_);
	check_error("alSourcef()");
}

void Source::set_rolloff_factor(float value)
{
	properties_.rolloff_factor_ = value;
	alSourcef(id_, AL_ROLLOFF_FACTOR, properties_.rolloff_factor_);
	check_error("alSourcef()");
}

void Source::set_velocity(glm::vec3 value)
{
	properties_.velocity_ = value;
	alSource3f(id_, AL_VELOCITY, properties_.velocity_.x, properties_.velocity_.y, properties_.velocity_.z);
	check_error("alSource3f()");
}

void Source::set_reference_distance(float value)
{
	properties_.reference_distance_ = value;
	alSourcef(id_, AL_REFERENCE_DISTANCE, properties_.reference_distance_);
	check_error("alSourcef()");
}

void Source::set_max_distance(float value)
{
	properties_.max_distance_ = value;
	alSourcef(id_, AL_MAX_DISTANCE, properties_.max_distance_);
	check_error("alSourcef()");
}

void Source::set_position(glm::vec3 value)
{
	properties_.position_ = value;
	alSource3f(id_, AL_POSITION, properties_.position_.x, properties_.position_.y, properties_.position_.z);
	check_error("alSource3f()");
}

}