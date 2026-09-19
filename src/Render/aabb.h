#pragma once

#include "gl_resource_manager.h"

#include <vector>

class AABB
{
	public:
		AABB(glm::vec3 min_values, glm::vec3 max_values, resource::MeshKey mesh_key);

		void draw();
		glm::vec3 get_min_values() const;
		glm::vec3 get_max_values() const;
		std::vector<glm::vec3> get_corners() const; //retourne une copie car le vector est une variable locale à la méthode

	private:
		resource::MeshKey mesh_key_;
		glm::vec3 max_values_, min_values_;
};