#pragma once

#include "Render/mesh.h"

#include <vector>

class AABB
{
	public:
		AABB(glm::vec3 min_values, glm::vec3 max_values, std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode);

		void draw();
		glm::vec3 get_min_values() const;
		glm::vec3 get_max_values() const;
		std::vector<glm::vec3> get_corners() const; //retourne une copie car le vector est une variable locale à la méthode

	private:
		Mesh mesh_;
		glm::vec3 max_values_, min_values_;
};