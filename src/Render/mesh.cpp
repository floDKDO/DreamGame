#include "mesh.h"
#include "Logging/logging.h"
#include "Resource/resource_manager.h"
#include "glTF/gltf.h"
#include "Common/utils.h"

#include <stb/stb_image.h>
#include <filesystem>
#include <iostream>

Mesh::Mesh(const Id& mesh_id, const Info& mesh_info)
	: mesh_id_(mesh_id), mesh_info_(mesh_info), ebo_(0), vbo_(0), vao_(0)
{
	load_mesh();
}

Mesh::Mesh(Mesh&& mesh)
	: mesh_id_(mesh.mesh_id_), mesh_info_(mesh.mesh_info_), ebo_(mesh.ebo_), vbo_(mesh.vbo_), vao_(mesh.vao_)
{
	mesh.ebo_ = 0; //glDeleteBuffers silently ignores 0's and names that do not correspond to existing buffer objects.
	mesh.vbo_ = 0; //idem
	mesh.vao_ = 0; //idem
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
	if(this == &mesh)
	{
		return *this;
	}

	destroy_all_buffers();

	mesh_id_ = mesh.mesh_id_;
	mesh_info_ = mesh.mesh_info_;
	ebo_ = mesh.ebo_;
	vbo_ = mesh.vbo_;
	vao_ = mesh.vao_;

	mesh.ebo_ = 0; //glDeleteBuffers silently ignores 0's and names that do not correspond to existing buffer objects.
	mesh.vbo_ = 0; //idem
	mesh.vao_ = 0; //idem
	return *this;
}

Mesh::~Mesh()
{
	destroy_all_buffers();
}

void Mesh::load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name)
{
	attribute::Info attribute_info = Vertex::get_attribute_info(attribute_name);

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VAO's attribute code.");
	glEnableVertexArrayAttrib(vao_, attribute_info.index_);
	glVertexArrayAttribFormat(vao_, attribute_info.index_, attribute_info.component_count_, GL_FLOAT, attribute_info.normalized_, GLuint(attribute_info.offset_));
	glVertexArrayAttribBinding(vao_, attribute_info.index_, vbo_binding_index);
	glPopDebugGroup();
}

unsigned char* Mesh::load_image(const ImageTexture& image_texture, int& width, int& height, int& channels) const
{
	unsigned char* pixels;
	if(std::holds_alternative<ImagePath>(image_texture.image_value_))
	{
		const ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);
		if((pixels = stbi_load(image_path.c_str(), &width, &height, &channels, 0)) == nullptr)
		{
			logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		const ImageData& image_data = std::get<ImageData>(image_texture.image_value_);
		if((pixels = stbi_load_from_memory(image_data.data(), int(image_data.size()), &width, &height, &channels, 0)) == nullptr)
		{
			logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
	return pixels;
}

void Mesh::create_ebo()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering EBO creation and storage code.");
	glCreateBuffers(1, &ebo_);
	glNamedBufferStorage(ebo_, mesh_info_.ebo_values_.size() * sizeof(mesh_info_.ebo_values_[0]), mesh_info_.ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
	glPopDebugGroup();
}

void Mesh::create_vbo()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VBO creation and storage code.");
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, mesh_info_.vertices_.get_vertices_number() * sizeof(Vertex), mesh_info_.vertices_.get_vertices_data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
	glPopDebugGroup();
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VAO creation, binding and content code.");
	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, GLsizei(sizeof(Vertex)));
	glVertexArrayElementBuffer(vao_, ebo_);
	glPopDebugGroup();

	if(mesh_info_.vertices_.has_position_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::POSITION);
	}

	if(mesh_info_.vertices_.has_normal_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::NORMAL);
	}

	if(mesh_info_.vertices_.has_texcoord_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::TEXCOORD);
	}

	if(mesh_info_.vertices_.has_color_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::COLOR);
	}
}

void Mesh::destroy_all_buffers()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering EBO, VBO and VAO deletion code.");
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_); //Attention : si j'utilise glDeleteBuffers() pour le vao, je vais avoir des problèmes !
	glPopDebugGroup();

	ebo_ = 0;
	vbo_ = 0;
	vao_ = 0;
}

void Mesh::init_texture(Texture* texture) const
{
	create_and_bind_texture(texture);
	set_texture_parameters(texture);
}

void Mesh::create_and_bind_texture(Texture* texture) const
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture creation and binding code.");
	glCreateTextures(texture->info_.target_, 1, &texture->info_.id_);
	glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::set_texture_storage(Texture* texture, int width, int height) const
{
	GLenum internal_format = GL_RGBA8;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		const ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		internal_format = gltf::get_sized_enum_from_channels(image_texture.channels_);
	}
	
	GLsizei number_of_texture_levels = texture->should_have_mipmaps_ ? 1 + int(std::floor(std::log2(std::max(width, height)))) : 1;
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture storage code.");
	glTextureStorage2D(texture->info_.id_, number_of_texture_levels, internal_format, width, height); //ici (cas .png), il faut mettre GL_RGBA8 et non GL_RGBA car il faut mettre un "Sized Internal Format" (voir https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml)
	glPopDebugGroup();
}

void Mesh::set_texture_parameters(const Texture* texture) const
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture parameters code.");
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_S, texture->info_.wrap_s_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_T, texture->info_.wrap_t_);
	if(texture->info_.target_ == GL_TEXTURE_CUBE_MAP) //TODO : ne pas oublier de modifier pour d'autres types de textures 3D
	{
		glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_R, texture->info_.wrap_r_);
	}
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MAG_FILTER, texture->info_.mag_filter_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MIN_FILTER, texture->info_.min_filter_);
	glPopDebugGroup();
}

void Mesh::set_texture_content(Texture* texture, int width, int height, void* pixels) const
{
	GLenum format = GL_RGBA;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		const ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		format = gltf::get_base_enum_from_channels(image_texture.channels_);
	}

	GLint texture_level = 0; //on ne spécifie que la texture de niveau 0 et on génère automatiquement les mipmaps avec glGenerateTextureMipmap()
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture content code.");
	if(texture->info_.target_ == GL_TEXTURE_2D)
	{
		glTextureSubImage2D(texture->info_.id_, texture_level, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels); //ici (cas .png), il faut mettre GL_RGBA et non GL_RGBA8, ce dernier n'étant pas supporté pour ce paramètre
	}
	glGenerateTextureMipmap(texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::set_cubemap_content(Texture* texture) const
{
	ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
	//if(std::holds_alternative<ImagePath>(image_texture.image_value_)) //TODO : toujours vrai normalement
	//{
		const ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);
		std::vector<std::string> files = utils::get_files_from_directory(image_path);
	//}

	GLint texture_level = 0; //on ne spécifie que la texture de niveau 0 et on génère automatiquement les mipmaps avec glGenerateTextureMipmap()
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering cubemap content code.");
	for(GLint i = 0; i < 6; ++i) //TODO : créer une constante
	{
		int width, height, channels;
		unsigned char* pixels;
		if((pixels = stbi_load(files[i].c_str(), &width, &height, &channels, 0)) == nullptr)
		{
			logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}

		if(i == 0) //TODO : dégueulasse
		{
			std::cout << "Width: " << width << ", height: " << height << std::endl;
			set_texture_storage(texture, width, height);
		}

		//TODO : ce code fonctionne uniquement si la convention de nommage des fichiers est respectée
		GLint z_offset = 0;
		if(files[i].find("right") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			z_offset = 0;
		}
		else if(files[i].find("left") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			z_offset = 1;
		}
		else if(files[i].find("top") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			z_offset = 2;
		}
		else if(files[i].find("bottom") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			z_offset = 3;
		}
		else if(files[i].find("back") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			z_offset = 5; //TODO : attention, la valeur est 5 et non 4
		}
		else if(files[i].find("front") != std::string::npos)
		{
			z_offset = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			z_offset = 4; //TODO : attention, la valeur est 4 et non 5
		}
		else
		{
			std::cout << "****************************************\n";
		}

		GLenum format = gltf::get_base_enum_from_channels(channels);
		//std::cout << "channels: " << format << ", z_offset: " << z_offset << ", width : " << width << ", height : " << height << ", format : " << format << std::endl;
		glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, z_offset, width, height, 1, format, GL_UNSIGNED_BYTE, pixels); //TODO : si j'utilise i, z_offset est inutile
		stbi_image_free(pixels);

		image_texture.channels_ = channels; //TODO : ces 3 lignes pourraient n'être exécutées qu'une seule fois
		image_texture.initial_width_ = width;
		image_texture.initial_height_ = height;
	}
	glGenerateTextureMipmap(texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::create_textures()
{
	for(std::size_t texture_id : mesh_info_.texture_ids_)
	{
		Texture* texture = resource::get_texture(texture_id);
		if(texture == nullptr)
		{
			logging::log("The texture is nullptr!", logging::Severity::WARNING);
			continue;
		}
		else if(std::holds_alternative<TextTexture>(texture->texture_))
		{ 
			if(TextTexture& text_texture = std::get<TextTexture>(texture->texture_); text_texture.width_ == 0 && text_texture.height_ == 0 && text_texture.pixels_ == nullptr)
			{
				logging::log("The text texture is empty!", logging::Severity::DEBUG);
				init_texture(texture);
				continue;
			}
		}
		
		if(texture->info_.target_ != GL_TEXTURE_CUBE_MAP)
		{
			int width, height;
			unsigned char* pixels = nullptr;
			if(std::holds_alternative<ImageTexture>(texture->texture_))
			{
				ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
				int channels;
				pixels = load_image(image_texture, width, height, channels);
				image_texture.channels_ = channels;
				image_texture.initial_width_ = width;
				image_texture.initial_height_ = height;
			}
			else if(std::holds_alternative<TextTexture>(texture->texture_))
			{
				const TextTexture& text_texture = std::get<TextTexture>(texture->texture_);
				pixels = static_cast<unsigned char*>(text_texture.pixels_);
				width = text_texture.width_;
				height = text_texture.height_;
			}
			else //cas qui n'arrive jamais
			{
				width = 0;
				height = 0;
			}

			init_texture(texture);
			set_texture_storage(texture, width, height);
			set_texture_content(texture, width, height, pixels);

			if(std::holds_alternative<ImageTexture>(texture->texture_))
			{
				stbi_image_free(pixels);
			}
		}
		else
		{
			//TODO : cubemaps => lier avec le reste du code
			//stbi_load(); //à faire 6 fois (une fois par face de la cube map)
			//modif des champs de image_texture; //à faire une seule fois pour une cube map (toutes les textures de la cube map ont les mêmes caractéristiques)
			//init_texture(texture); //à faire une seule fois pour une cube map
			//set_texture_storage(texture, width, height); //à faire une seule fois pour une cube map
			//glGenerateTextureMipmap(texture->info_.id_); //dans méthode set_texture_content() : à faire une seule fois pour une cube map
			//glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, 1, format, GL_UNSIGNED_BYTE, pixels); //dans méthode set_texture_content() : à faire 6 fois (une fois par face de la cube map)
			//stbi_image_free(pixels); //à faire 6 fois (une fois par face de la cube map)

			init_texture(texture);
			//for(GLint i = 0; i < 6; ++i)
			{
				set_cubemap_content(texture);
			}
		}
	}
}

void Mesh::load_mesh()
{
	create_ebo();
	create_vbo();
	create_vao();
	create_textures();
}

void Mesh::edit_text_texture(int new_width, int new_height, void* new_pixels) const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'un Text ne contient qu'une seule texture
	TextTexture& texture_text = std::get<TextTexture>(texture->texture_);

	int old_width, old_height;
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture level parameters code.");
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_WIDTH, &old_width);
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_HEIGHT, &old_height);
	glPopDebugGroup();

	if(new_width != old_width || new_height != old_height)
	{
		texture_text.width_ = new_width;
		texture_text.height_ = new_height;

		glDeleteTextures(1, &texture->info_.id_);
		init_texture(texture);
		set_texture_storage(texture, new_width, new_height);
	}

	texture_text.pixels_ = new_pixels;
	set_texture_content(texture, new_width, new_height, new_pixels);
}

int Mesh::get_initial_texture_width() const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'une Image ne contient qu'une seule texture
	const ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_width_;
}

int Mesh::get_initial_texture_height() const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'une Image ne contient qu'une seule texture
	const ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_height_;
}

void Mesh::render() const
{
	if(ShaderProgram* shader_program = resource::get_currently_bound_shader(); shader_program != nullptr)
	{
		for(std::size_t texture_id : mesh_info_.texture_ids_)
		{
			Texture* texture = resource::get_texture(texture_id);
			if(texture->info_.texture_unit_ > 0)
			{
				logging::log("Only one texture by mesh for now!", logging::Severity::WARNING);
			}
			//std::cout << "BIND\n";
			glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
			//std::cout << "END BIND\n";
			//std::cout << "SAMPLER: " << shader_program->get_shader_program_name() << ", num: " << mesh_info_.texture_keys_.size() << std::endl;
			if(texture->info_.target_ != GL_TEXTURE_CUBE_MAP)
			{
				shader_program->set_uniform_1i("texture_sampler0_", texture->info_.texture_unit_);
			}
			else
			{
				shader_program->set_uniform_1i("cubemap_sampler0_", texture->info_.texture_unit_);
			}
		}
	}
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering binding VAO and drawing code.");
	glBindVertexArray(vao_);
	glDrawElements(mesh_info_.render_mode_, GLsizei(mesh_info_.ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
	glPopDebugGroup();
}