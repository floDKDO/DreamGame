#pragma once

#include <glm/vec3.hpp>
#include <string_view>
#include <string>

namespace audio
{

void set_listener_position(glm::vec3 position);
void set_listener_velocity(glm::vec3 velocity);
void set_listener_orientation(glm::vec3 forward, glm::vec3 up);
void create_source(const std::string& model_name, std::string_view path);
void destroy_source(const std::string& model_name);
void set_source_pitch(const std::string& model_name, float value);
void set_source_gain(const std::string& model_name, float value);
void set_source_reference_distance(const std::string& model_name, float value);
void set_source_position(const std::string& model_name, glm::vec3 position);
void set_source_max_distance(const std::string& model_name, float value);
void set_source_rolloff_factor(const std::string& model_name, float value);
void set_source_looping(const std::string& model_name);
void play_source(const std::string& model_name);

}