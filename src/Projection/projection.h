#pragma once

#include <glm/mat4x4.hpp>

namespace projection
{

glm::mat4 get_perspective_matrix();
glm::mat4 get_orthographic_matrix();

}