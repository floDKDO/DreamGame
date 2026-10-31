#include "projection.h"
#include "Common/utils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace projection
{

glm::mat4 get_perspective_matrix()
{
	return glm::perspective(glm::radians(45.0f), float(utils::get_viewport_width()) / float(utils::get_viewport_height()), 0.1f, 100.0f); //TODO : hardcodé
}

glm::mat4 get_orthographic_matrix()
{
	//TODO : créer des constantes pour 1280.0f et 720.0f
	return glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f); //l'origine (0, 0) est en haut à gauche => pas besoin d'appeler stbi_set_flip_vertically_on_load(true);
}

}