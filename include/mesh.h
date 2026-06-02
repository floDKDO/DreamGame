#include "gl/glew.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Mesh
{
	public:
		Mesh(std::vector<GLushort> ebo_values, std::vector<glm::vec3> position_attribute_values);
	
	private:

};