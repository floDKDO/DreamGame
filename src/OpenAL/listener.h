#pragma once

#include <glm/vec3.hpp>

namespace audio
{

void set_listener_position(glm::vec3 position);
void set_listener_velocity(glm::vec3 velocity);
void set_listener_orientation(glm::vec3 forward, glm::vec3 up);

}