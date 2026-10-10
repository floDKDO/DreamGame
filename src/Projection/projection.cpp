#include "projection.h"
#include "Common/utils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace projection
{

glm::mat4 get_perspective_matrix()
{
	return glm::perspective(glm::radians(45.0f), float(utils::get_viewport_width()) / float(utils::get_viewport_height()), 0.1f, 100.0f);
}

glm::mat4 get_orthographic_matrix()
{
	return glm::ortho(0.0f, float(utils::get_viewport_width()), float(utils::get_viewport_height()), 0.0f, -1.0f, 1.0f);
}

}