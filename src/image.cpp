#include "image.h"
#include "Resource/gl_resource_manager.h"

Image::Image(std::string_view path)
{
	std::string path_str = std::string(path);

	std::vector<GLushort> ebo_values{0, 1, 2, 0, 3, 1};

	std::vector<glm::vec3> position_vector //on simule un vec2 avec un vec3 => la composante z vaut donc 0.0f
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};
	std::vector<glm::vec4> color_vector //couleur blanche <=> aucune couleur
	{
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	};
	std::vector<glm::vec2> texcoord_vector
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	Vertices vertices(4);
	vertices.add_position_attributes(position_vector);
	vertices.add_color_attributes(color_vector);
	vertices.add_texcoord_attributes(texcoord_vector);

	Texture texture{0, 0, path_str};
	std::string texture_key = resource::add_texture(texture);

	mesh_id_ = resource::add_mesh(Mesh::Id{path_str, -1}, Mesh::Info{ebo_values, vertices, {texture_key}, GL_TRIANGLES});
}

void Image::render() const
{
	if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->render();
	}
}