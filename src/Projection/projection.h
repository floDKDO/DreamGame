#pragma once

#include <glm/mat4x4.hpp>

namespace projection
{

glm::mat4 get_perspective_matrix(float aspect);
glm::mat4 get_orthographic_matrix(float window_width, float window_height);

}