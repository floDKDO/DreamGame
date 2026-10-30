#include "resource_manager.h"
#include "resources.h"
#include "model.h"
#include "Common/utils.h"
#include "Logging/logging.h"

#include <stb/stb_image.h>
#include <string>
#include <iostream>

namespace resource
{

std::unordered_map<std::string, ShaderProgram> shader_programs_; //pas besoin d'id de type std::size_t
Resources<std::size_t, Model> models_; //template specialization (voir "resources.h")
Resources<Mesh::Id, Mesh> meshes_;
Resources<Mesh::Id, Mesh> aabb_meshes_;
Resources<sdl::Font::Id, sdl::Font> fonts_;
Resources<std::string, Texture> textures_; //si la texture possède un path, sa clef est le path ; si la texture est au format embedded glTF, toujours l'insérer cas peu commun et pas vraiment possible d'identifer de manière unique ce type de texture sans lire tout leur contenu)

//Ces méthodes sont placées là (donc pas dans le header) car elles sont utilisées uniquement dans ce fichier .cpp
ShaderProgram* get_shader(std::string_view name);
std::size_t get_id_from_font_id(sdl::Font::Id font_id);


void destroy_all_resources()
{
	for(auto& [name, texture] : textures_.resources_)
	{
		if(std::holds_alternative<ImageTexture>(texture.specific_info_))
		{
			ImageTexture& image_texture = std::get<ImageTexture>(texture.specific_info_);
			stbi_image_free(image_texture.pixels_);
		}
	}
	shader_programs_.clear();
	models_.clear();
	meshes_.clear();
	aabb_meshes_.clear();
	fonts_.clear();
	textures_.clear();
}

std::size_t add_texture(Texture&& texture)
{
	std::string texture_key;
	if(std::holds_alternative<ImageTexture>(texture.specific_info_))
	{
		ImageTexture& image_texture = std::get<ImageTexture>(texture.specific_info_);
		if(!image_texture.path_.empty())
		{
			texture_key = image_texture.path_;
		}
		else
		{
			//Comme les images "embedded glTF" n'ont pas de path, la clef étant "", elle ne serait pas unique pour plusieurs images sans path
			//Pour assurer que chacune de ces images aient un path fictif unique, je lui ajoute la valeur du compteur global (incrémenté à chaque ajout) => le path n'étant pas consulté donc la valeur de cette clef n'a aucune importance
			//De toute façon, pour ce type d'image, aucune vérification n'est effectuée : elles sont ajoutées dans tous les cas dans textures_
			texture_key = "Empty path " + std::to_string(GlobalId::get());
		}
	}
	else if(std::holds_alternative<TextTexture>(texture.specific_info_))
	{
		//Idem ici pour les textures de type "text"
		texture_key = "Text " + std::to_string(GlobalId::get());
	}

	return textures_.insert_resource(texture_key, std::move(texture));
}

Texture* get_texture(std::size_t id)
{
	if(textures_.ids_.count(id))
	{
		if(std::string texture_string = textures_.ids_.at(id); textures_.resources_.count(texture_string))
		{
			return &textures_.resources_.at(texture_string);
		}
	}
	logging::log("get_texture() returned nullptr!", logging::Severity::WARNING);
	return nullptr;
}

std::size_t add_2d_text(std::string_view text, const sdl::Surface& surface)
{
	int width = text.empty() ? 0 : surface.get_width();
	int height = text.empty() ? 0 : surface.get_height();

	Texture texture{TextureInfo{false}, SamplingParameters{}, width, height, 4, text.empty() ? TextTexture{} : TextTexture{surface.get_pixels()}};
	std::size_t texture_id = add_texture(std::move(texture));
	return add_mesh(text, Mesh::Info{utils::get_2d_ebo_values(), utils::get_2d_vertices(), {texture_id}, GL_TRIANGLES});
}

std::size_t add_2d_image(std::string_view path)
{
	Texture texture{TextureInfo{}, SamplingParameters{}};

	ImageTexture image_texture{std::string(path)};
	image_texture.pixels_ = utils::checked_stbi_load(image_texture.path_, texture.original_width_, texture.original_height_, texture.original_channels_);
	texture.specific_info_ = image_texture;

	std::size_t texture_id = add_texture(std::move(texture));
	return add_mesh(path, Mesh::Info{utils::get_2d_ebo_values(), utils::get_2d_vertices(), {texture_id}, GL_TRIANGLES});
}

std::size_t add_cubemap(std::string_view directory_path)
{
	SamplingParameters sampling_parameters;
	sampling_parameters.wrap_s_ = GL_CLAMP_TO_EDGE;
	sampling_parameters.wrap_t_ = GL_CLAMP_TO_EDGE;
	sampling_parameters.wrap_r_ = GL_CLAMP_TO_EDGE;

	std::string file_extension = utils::get_extension_from_cubemap_directory(directory_path);
	std::string directory_separator = directory_path[directory_path.size() - 1] == '/' ? "" : "/";
	std::string directory_path_str = std::string(directory_path) + directory_separator;
	int width, height, channels;

	std::string path_back = directory_path_str + "back" + file_extension;
	unsigned char* pixels_back = utils::checked_stbi_load(path_back, width, height, channels);
	ImageTexture image_texture_back{path_back, pixels_back};

	std::string path_bottom = directory_path_str + "bottom" + file_extension;
	ImageTexture image_texture_bottom{path_bottom, utils::checked_stbi_load(path_bottom)};

	std::string path_front = directory_path_str + "front" + file_extension;
	ImageTexture image_texture_front{path_front, utils::checked_stbi_load(path_front)};

	std::string path_left = directory_path_str + "left" + file_extension;
	ImageTexture image_texture_left{path_left, utils::checked_stbi_load(path_left)};

	std::string path_right = directory_path_str + "right" + file_extension;
	ImageTexture image_texture_right{path_right, utils::checked_stbi_load(path_right)};

	std::string path_top = directory_path_str + "top" + file_extension;
	ImageTexture image_texture_top{path_top, utils::checked_stbi_load(path_top)};

	CubemapTexture cubemap_texture;
	cubemap_texture.back_ = image_texture_back;
	cubemap_texture.bottom_ = image_texture_bottom;
	cubemap_texture.front_ = image_texture_front;
	cubemap_texture.left_ = image_texture_left;
	cubemap_texture.right_ = image_texture_right;
	cubemap_texture.top_ = image_texture_top;

	Texture texture{TextureInfo{true, GL_TEXTURE_CUBE_MAP}, sampling_parameters, width, height, channels, cubemap_texture};
	std::size_t texture_id = add_texture(std::move(texture));
	return add_mesh(directory_path, Mesh::Info{utils::get_3d_ebo_values(), utils::get_3d_vertices(), {texture_id}, GL_TRIANGLES});
}

const Mesh* get_mesh(std::size_t id)
{
	if(meshes_.ids_.count(id))
	{
		if(Mesh::Id mesh_id = meshes_.ids_.at(id); meshes_.resources_.count(mesh_id))
		{
			return &meshes_.resources_.at(mesh_id);
		}
	}
	logging::log("get_mesh() returned nullptr!", logging::Severity::WARNING);
	return nullptr;
}

std::size_t add_mesh(std::string_view file_name, Mesh::Info mesh_info, int32_t mesh_index)
{
	Mesh::Id mesh_id{std::string(file_name), mesh_index};
	return meshes_.insert_resource(mesh_id, Mesh(mesh_id, mesh_info));
}

std::size_t add_aabb_mesh(std::string_view file_name, Mesh::Info mesh_info, int32_t mesh_index)
{
	Mesh::Id mesh_id{std::string(file_name), mesh_index};
	return aabb_meshes_.insert_resource(mesh_id, Mesh(mesh_id, mesh_info));
}

const Mesh* get_aabb_mesh(std::size_t id)
{
	if(aabb_meshes_.ids_.count(id))
	{
		if(Mesh::Id mesh_id = aabb_meshes_.ids_.at(id); aabb_meshes_.resources_.count(mesh_id))
		{
			return &aabb_meshes_.resources_.at(mesh_id);
		}
	}
	logging::log("get_aabb_mesh() returned nullptr!", logging::Severity::WARNING);
	return nullptr;
}

std::size_t add_model(std::string_view path, Transform transform)
{
	return models_.insert_resource(Model(path, transform));
}

std::size_t add_model(std::string_view path)
{
	return models_.insert_resource(Model(path));
}

Model* get_model(std::size_t id)
{
	if(models_.resources_.count(id))
	{
		return &models_.resources_.at(id);
	}
	else
	{
		logging::log("get_model() returned nullptr!", logging::Severity::WARNING);
		return nullptr;
	}
}

std::unordered_map<std::size_t, Model>& get_models()
{
	return models_.resources_;
}

void add_shader(std::string_view name, std::vector<std::string> shader_paths)
{
	shader_programs_.insert(std::make_pair(name, ShaderProgram(name, shader_paths)));
}

ShaderProgram* bind_shader(std::string_view name)
{
	ShaderProgram* shader_program = get_shader(name);

	GLint shader_program_id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id);
	if(GLuint(shader_program_id) == shader_program->get_shader_program_id())
	{
		//logging::log("Trying to bind to an already bound shader (" + std::string(name) + ")", logging::Severity::NOTICE);
		return shader_program;
	}
	shader_program->use();
	return shader_program;
}

ShaderProgram* get_shader(std::string_view name)
{
	std::string name_str = std::string(name);
	if(shader_programs_.count(name_str))
	{
		return &shader_programs_.at(name_str);
	}
	else
	{
		logging::log("get_shader() returned nullptr!", logging::Severity::WARNING);
		return nullptr;
	}
}

ShaderProgram* get_currently_bound_shader()
{
	GLint shader_program_id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id);

	for(auto& [shader_program_name, shader_program] : shader_programs_)
	{
		if(shader_program.get_shader_program_id() == GLuint(shader_program_id))
		{
			return &shader_program;
		}
	}
	logging::log("get_currently_bound_shader() returned nullptr!", logging::Severity::WARNING);
	return nullptr;
}

std::size_t get_id_from_font_id(sdl::Font::Id font_id)
{
	for(auto& [res_id, res_font_id] : fonts_.ids_)
	{
		if(res_font_id == font_id)
		{
			return res_id;
		}
	}
	return std::numeric_limits<std::size_t>::max();
}

std::size_t add_font(std::string_view font_path, float font_size)
{
	for(auto& [res_font_id, res_font] : fonts_.resources_)
	{
		if(font_path == res_font_id.font_path_)
		{
			sdl::Font::Id font_id{res_font_id.font_path_, font_size};
			if(font_size != res_font_id.font_size_)
			{
				return fonts_.insert_resource(font_id, sdl::Font(res_font.copy(), font_size));
			}
			else
			{
				return get_id_from_font_id(font_id);
			}
		}
	}
	sdl::Font::Id font_id{std::string(font_path), font_size};
	return fonts_.insert_resource(font_id, sdl::Font(font_path, font_size));
}

sdl::Font* get_font(std::size_t id)
{
	if(fonts_.ids_.count(id))
	{
		if(sdl::Font::Id font_id = fonts_.ids_.at(id); fonts_.resources_.count(font_id))
		{
			return &fonts_.resources_.at(font_id);
		}
	}
	logging::log("get_font() returned nullptr!", logging::Severity::WARNING);
	return nullptr;
}

}