#include "image.h"
#include "Resource/resource_manager.h"

#include <glm/gtc/type_ptr.hpp>

Image::Image(std::string_view path)
	: position_(glm::vec2(0.0f)), angle_(0.0f), mesh_id_(resource::add_2d_image(path))
{
	mesh_id_ = resource::add_2d_image(path);

	const Mesh* mesh = resource::get_mesh(mesh_id_);
	initial_texture_width_ = mesh->get_initial_texture_width();
	initial_texture_height_ = mesh->get_initial_texture_height();
	size_ = glm::vec2(initial_texture_width_, initial_texture_height_);
}

glm::vec2 Image::get_position() const
{
	return position_;
}

float Image::get_width() const
{
	return size_.x;
}

float Image::get_height() const
{
	return size_.y;
}

//convention : l'origine est en haut à gauche de l'écran
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