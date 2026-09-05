#pragma once

#include "Render/mesh.h"

#include <vector>

class AABB
{
	public:
		AABB(glm::vec3 min_values, glm::vec3 max_values, std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture> textures, GLenum draw_mode);
		AABB(glm::vec3 min_values, glm::vec3 max_values, std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode);
		void draw(ShaderProgram& shader_program);
		glm::vec3 get_min_values() const;
		glm::vec3 get_max_values() const;
		std::vector<glm::vec3> get_corners() const;

	private:
		Mesh mesh_;
		glm::vec3 max_values_, min_values_;
};