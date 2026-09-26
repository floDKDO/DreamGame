#include "projection.h"

#include <glm/gtc/matrix_transform.hpp>

namespace projection
{

glm::mat4 get_perspective_matrix(float aspect)
{
	return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

}