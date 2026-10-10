#include "text.h"
#include "Resource/resource_manager.h"

#include <glm/gtc/type_ptr.hpp>
#include <SDL3_ttf/SDL_ttf.h>

//TODO : constructor delegation
Text::Text(std::string_view text)
	: color_({255, 255, 255, 255}), is_wrapped_(false), wrap_length_(0), quality_(Quality::SOLID), 
	font_id_(resource::add_font("resources/fonts/Aller_Rg.ttf", 24.0f)), //TODO : police hardcodée
	text_(text), position_(glm::vec2(0.0f)), angle_(0.0f)
{
	init_surface_from_text();

	std::string text_str = std::string(text);
	
	std::vector<GLushort> ebo_values{0, 1, 2, 0, 3, 1};

	std::vector<glm::vec3> position_vector //on simule un vec2 avec un vec3 => la composante z vaut donc 0.0f
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};
	/*std::vector<glm::vec4> color_vector //TODO : à décommenter si je souhaite une couleur de fond pour le texte
	{
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)
	};*/
	std::vector<glm::vec2> texcoord_vector
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	Vertices vertices(4);
	vertices.add_position_attributes(position_vector);
	//vertices.add_color_attributes(color_vector);
	vertices.add_texcoord_attributes(texcoord_vector);

	Texture texture{TextureInfo{}, TextTexture{surface_.get_width(), surface_.get_height(), surface_.get_pixels()}, false};
	std::string texture_key = resource::add_texture(texture);

	mesh_id_ = resource::add_mesh(Mesh::Id{text_str, -1}, Mesh::Info{ebo_values, vertices, {texture_key}, GL_TRIANGLES});
}

Text::Text(std::string_view text, int wrap_length)
	: color_({255, 255, 255, 255}), is_wrapped_(true), wrap_length_(wrap_length), quality_(Quality::SOLID),
	font_id_(resource::add_font("resources/fonts/Aller_Rg.ttf", 24.0f)), //TODO : police hardcodée
	text_(text), position_(glm::vec2(0.0f)), angle_(0.0f)
{
	init_surface_from_text();

	std::string text_str = std::string(text);

	std::vector<GLushort> ebo_values{0, 1, 2, 0, 3, 1};

	std::vector<glm::vec3> position_vector //on simule un vec2 avec un vec3 => la composante z vaut donc 0.0f
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};
	/*std::vector<glm::vec4> color_vector //TODO : à décommenter si je souhaite une couleur de fond pour le texte
	{
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)
	};*/
	std::vector<glm::vec2> texcoord_vector
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	Vertices vertices(4);
	vertices.add_position_attributes(position_vector);
	//vertices.add_color_attributes(color_vector);
	vertices.add_texcoord_attributes(texcoord_vector);

	if(!text.empty())
	{
		Texture texture{TextureInfo{}, TextTexture{surface_.get_width(), surface_.get_height(), surface_.get_pixels()}, false};
		std::string texture_key = resource::add_texture(texture);

		mesh_id_ = resource::add_mesh(Mesh::Id{text_str, -1}, Mesh::Info{ebo_values, vertices, {texture_key}, GL_TRIANGLES});
	}
	else
	{
		Texture texture{TextureInfo{}, TextTexture{}, false};
		std::string texture_key = resource::add_texture(texture);

		mesh_id_ = resource::add_mesh(Mesh::Id{text_str, -1}, Mesh::Info{ebo_values, vertices, {texture_key}, GL_TRIANGLES});
	}
}

void Text::init_surface_from_text()
{
	sdl::Font* font = resource::get_font(font_id_.font_path_, font_id_.font_size_);

	//la valeur "{255, 255, 255, 255}" n'est pas hardcodée : elle est nécessaire pour que la multiplication via text_color_ fonctionne
	if(quality_ == Quality::SOLID)
	{
		if(is_wrapped_)
		{
			surface_.render_text_solid_wrapped(*font, text_, {255, 255, 255, 255}, wrap_length_);
		}
		else
		{
			surface_.render_text_solid(*font, text_, {255, 255, 255, 255});
		}
	}
	else
	{
		if(is_wrapped_)
		{
			surface_.render_text_lcd_wrapped(*font, text_, {255, 255, 255, 255}, wrap_length_);
		}
		else
		{
			surface_.render_text_lcd(*font, text_, {255, 255, 255, 255});
		}
	}
}

void Text::set_wrapped(int wrap_length)
{
	is_wrapped_ = true;
	wrap_length_ = wrap_length;
	init_surface_from_text();
	//std::cout << "SET WRAPPED\n";
	resource::get_mesh(mesh_id_)->edit_text_texture(surface_.get_width(), surface_.get_height(), surface_.get_pixels());
}

void Text::set_position(glm::vec2 position)
{
	position_ = position;
}

void Text::set_font_size(float font_size)
{
	font_id_ = resource::add_font(font_id_.font_path_, font_size);
	init_surface_from_text();
	//std::cout << "SET FONT SIZE\n";
	resource::get_mesh(mesh_id_)->edit_text_texture(surface_.get_width(), surface_.get_height(), surface_.get_pixels());
}

void Text::set_angle(float angle)
{
	angle_ = angle;
}

void Text::edit_text(std::string_view new_text)
{
	text_ = new_text;
	init_surface_from_text();
	//std::cout << "SET EDIT TEXT\n";
	resource::get_mesh(mesh_id_)->edit_text_texture(surface_.get_width(), surface_.get_height(), surface_.get_pixels());
}

void Text::clear()
{
	text_.clear();
	//init_surface_from_text();
	resource::get_mesh(mesh_id_)->clear_text_texture();
}

void Text::add_char(char c)
{
	text_ += c;
	init_surface_from_text();
	//std::cout << "ADD CHAR\n";
	resource::get_mesh(mesh_id_)->edit_text_texture(surface_.get_width(), surface_.get_height(), surface_.get_pixels());
}

glm::mat4 Text::get_model_matrix() const
{
	float width = float(surface_.get_width());
	float height = float(surface_.get_height());

	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(position_, 0.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(angle_), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));
	model_matrix = glm::scale(model_matrix, glm::vec3(width, height, 0.0f));
	return model_matrix;
}

void Text::render() const
{
	if(!text_.empty())
	{
		if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if(ShaderProgram* shader_program_2d = resource::bind_shader("Text"); shader_program_2d != nullptr)
			{
				shader_program_2d->set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(get_model_matrix()));
				shader_program_2d->set_uniform_4f("text_color_", glm::vec4(color_.r, color_.g, color_.b, color_.a));
			}
			mesh->render();
		}
	}
}