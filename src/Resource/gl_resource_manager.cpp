#include "gl_resource_manager.h"
#include "Logging/logging.h"
#include "Resource/model.h"

#include <string>
#include <iostream>

namespace resource
{

static std::size_t global_model_id_ = 0ULL;

std::unordered_map<Mesh::Id, Mesh> meshes_;
std::unordered_map<Mesh::Id, Mesh> aabb_meshes_;
std::unordered_map<std::string, ShaderProgram> shader_programs_;
std::unordered_map<std::size_t, Model> models_;

//Conteneur de texture, texture id 
// => cas texture dont l'image possède un path : la clef est le path (= chemin de la texture)
// => cas texture dont l'image a ses données en base64 : (toujours insérer la texture car cas peu commun et pas vraiment possible d'identifer de manière unique ce type de texture sans lire tout leur contenu)
std::unordered_map<std::string, Texture> textures_;

//Cette méthode est placée là (donc pas dans le header) car elle est utilisée uniquement dans ce fichier .cpp
ShaderProgram* get_shader(std::string_view name);

std::string add_texture(Texture texture)
{
	std::string texture_key;
	if(texture.image_path_.empty())
	{
		//Comme les images "embedded glTF" n'ont pas de path, la clef étant "", elle ne serait pas unique pour plusieurs images sans path
		//Pour assurer que chacune de ces images aient un path fictif unique, je lui ajoute un entier (incrémenté à chaque ajout) => le path n'étant pas consulté donc la valeur de cette clef n'a aucune importance
		//De toute façon, pour ce type d'images, aucune vérification n'est effectuée : elles sont ajoutées dans tous les cas dans textures_ 
		static std::size_t counter = 0ULL;
		texture_key = "Empty path " + std::to_string(counter);
		textures_.insert({texture_key, texture});
		counter += 1;
	}
	else
	{
		texture_key = texture.image_path_;
		textures_.insert({texture_key, texture});
	}
	return texture_key;
}

Texture* get_texture(std::string_view texture_key)
{
	std::string texture_key_str = std::string(texture_key);
	if(textures_.count(texture_key_str))
	{
		return &textures_.at(texture_key_str);
	}
	else
	{
		return nullptr;
	}
}

Mesh::Id add_mesh(Mesh::Id mesh_id, Mesh::Info mesh_info)
{
	meshes_.insert(std::make_pair(mesh_id, Mesh(mesh_id, mesh_info)));
	return mesh_id;
}

const Mesh* get_mesh(Mesh::Id mesh_id)
{
	if(meshes_.count(mesh_id))
	{
		return &meshes_.at(mesh_id);
	}
	else
	{
		return nullptr;
	}
}

Mesh::Id add_aabb_mesh(Mesh::Id mesh_id, Mesh::Info mesh_info)
{
	aabb_meshes_.insert(std::make_pair(mesh_id, Mesh(mesh_id, mesh_info)));
	return mesh_id;
}

const Mesh* get_aabb_mesh(Mesh::Id aabb_mesh_id)
{
	if(aabb_meshes_.count(aabb_mesh_id))
	{
		return &aabb_meshes_.at(aabb_mesh_id);
	}
	else
	{
		return nullptr;
	}
}

std::size_t add_model(std::string_view path, Transform transform)
{
	std::size_t local_model_id = global_model_id_;
	auto pair = models_.insert({local_model_id, Model(path, transform)});
	//std::cout << "ADD => " << path << ", " << local_model_id << std::endl;
	if(pair.second)
	{
		global_model_id_ += 1;
	}
	return local_model_id;
}

std::size_t add_model(std::string_view path)
{
	std::size_t local_model_id = global_model_id_;
	auto pair = models_.insert({local_model_id, Model(path)});
	//std::cout << "ADD => " << path << ", " << local_model_id << std::endl;
	if(pair.second)
	{
		global_model_id_ += 1;
	}
	return local_model_id;
}

Model* get_model(std::size_t model_id)
{
	if(models_.count(model_id))
	{
		return &models_.at(model_id);
	}
	else
	{
		return nullptr;
	}
}

std::unordered_map<std::size_t, Model>& get_models()
{
	return models_;
}

void add_shader(std::string_view name, std::vector<std::string> shader_path)
{
	shader_programs_.insert(std::make_pair(name, ShaderProgram(name, shader_path)));
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
	return nullptr;
}

}