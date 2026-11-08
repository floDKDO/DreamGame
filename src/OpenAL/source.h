#pragma once

#include "glm/vec3.hpp"

#include <AL/al.h>
#include <string_view>

namespace al
{

class Source
{
	public:
		struct Properties
		{
			float gain_ = 1.0f;
			float pitch_ = 1.0f;
			float rolloff_factor_ = 1.0f;
			glm::vec3 velocity_ = glm::vec3(0.0f);
			bool loop_ = false;
			float reference_distance_ = 1.0f;
			float max_distance_ = 20.0f;
			glm::vec3 position_ = glm::vec3(0.0f);
		};

		Source(std::size_t buffer_id, bool loop);
		Source(std::size_t buffer_id, glm::vec3 position, bool loop);
		Source(const Source&) = delete;
		Source(Source&& source);
		Source& operator=(const Source&) = delete;
		Source& operator=(Source&& source);
		~Source();

		void play() const;
		void destroy() const;
		void set_gain(float value);
		void set_pitch(float value);
		void set_rolloff_factor(float value);
		void set_velocity(glm::vec3 value);
		void set_reference_distance(float value);
		void set_max_distance(float value);
		void set_position(glm::vec3 value);

	private:
		void init();

		ALuint id_;
		std::size_t buffer_id_;
		Properties properties_;
		ALsizei size_;
};

}