#include "image.h"
#include "Resource/gl_resource_manager.h"

#include <glm/gtc/type_ptr.hpp>

Image::Image(std::string_view path, glm::vec2 position, glm::vec2 size, float angle)
	: position_(position), size_(size), angle_(angle)
{
	std::string path_str = std::string(path);

	//TODO : code dupliqué avec la classe Image
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

	Texture texture{TextureInfo{}, ImageTexture{path_str}, false};
	std::string texture_key = resource::add_texture(texture);

	mesh_id_ = resource::add_mesh(Mesh::Id{path_str, -1}, Mesh::Info{ebo_values, vertices, {texture_key}, GL_TRIANGLES});
}

void Image::set_position(glm::vec2 position)
{
	position_ = position;
}

void Image::set_angle(float angle)
{
	angle_ = angle;
}

void Image::set_size(glm::vec2 size)
{
	size_ = size;
}

glm::mat4 Image::get_model_matrix() const
{
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(position_, 0.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * size_.x, 0.5f * size_.y, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(angle_), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * size_.x, -0.5f * size_.y, 0.0f));
	model_matrix = glm::scale(model_matrix, glm::vec3(size_, 0.0f));
	return model_matrix;
}

void Image::render() const
{
	if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(ShaderProgram* shader_program_2d = resource::bind_shader("2d"); shader_program_2d != nullptr)
		{
			shader_program_2d->set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(get_model_matrix()));
		}
		mesh->render();
	}
}