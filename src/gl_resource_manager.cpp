#include "gl_resource_manager.h"
#include "Logging/logging.h"
#include "Render/model.h"

#include <map>
#include <unordered_map>
#include <string>
#include <iostream>

namespace resource
{

std::map<Mesh::MeshId, Mesh> meshes_; //TODO : ne marche pas avec une std::unordered_map
std::map<Mesh::MeshId, Mesh> aabb_meshes_; //TODO : ne marche pas avec une std::unordered_map
std::unordered_map<std::string, ShaderProgram> shader_programs_;
std::unordered_map<std::string, GLint> uniforms_;
std::unordered_map<std::string, Model> models_;

//Conteneur de texture, texture id 
// => cas texture dont l'image possède un path : la clef est le path (= chemin de la texture)
// => cas texture dont l'image a ses données en base64 : (toujours insérer la texture car cas peu commun et pas vraiment possible d'identifer de manière unique ce type de texture sans lire tout leur contenu)
std::unordered_map<std::string, Texture> textures_;

std::string add_texture(Texture texture)
{
	std::string texture_key;
	if(texture.image_path_.empty())
	{
		//Comme les images "embedded glTF" n'ont pas de path, la clef étant "", elle ne serait pas unique pour plusieurs images sans path
		//Pour assurer que chacune de ces images aient un path fictif unique, je lui ajoute un entier (incrémenté à chaque ajout) => le path n'étant pas consulté donc la valeur de cette clef n'a aucune importance
		//De toute façon, pour ce type d'images, aucune vérification n'est effectuée : elles sont ajoutées dans tous les cas dans textures_ 
		static std::size_t counter = 0;
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

Texture* get_texture(std::string texture_key)
{
	if(textures_.count(texture_key))
	{
		return &textures_.at(texture_key);
	}
	else
	{
		return nullptr;
	}
}

Mesh::MeshId add_mesh(Mesh::MeshId mesh_id, Mesh::MeshInfo mesh_info)
{
	meshes_.insert(std::make_pair(mesh_id, Mesh(mesh_id, mesh_info)));
	return mesh_id;
}

const Mesh* get_mesh(Mesh::MeshId mesh_id)
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

Mesh::MeshId add_aabb_mesh(Mesh::MeshId mesh_id, Mesh::MeshInfo mesh_info)
{
	aabb_meshes_.insert(std::make_pair(mesh_id, Mesh(mesh_id, mesh_info)));
	return mesh_id;
}

const Mesh* get_aabb_mesh(Mesh::MeshId aabb_mesh_id)
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

std::string add_model(std::string_view path, Transform transform)
{
	std::string name_str = std::string(path);
	models_.insert({name_str, Model(path, transform)}); //TODO : pour l'instant, la clef (= name) vaut le path
	return name_str;
}

std::string add_model(std::string_view path)
{
	std::string name_str = std::string(path);
	models_.insert({name_str, Model(path)}); //TODO : pour l'instant, la clef (= name) vaut le path
	return name_str;
}

Model* get_model(std::string_view name) //TODO : pour l'instant, la clef (= name) vaut le path
{
	std::string name_str = std::string(name);
	if(models_.count(name_str))
	{
		return &models_.at(name_str);
	}
	else
	{
		return nullptr;
	}
}

void add_shader(std::string_view name, std::vector<std::string> shader_path)
{
	shader_programs_.insert(std::make_pair(name, ShaderProgram(name, shader_path)));
}

const ShaderProgram& get_shader(std::string_view name)
{
	std::string name_str = std::string(name);
	if(shader_programs_.count(name_str))
	{
		return shader_programs_.at(name_str);
	}
	else
	{
		logging::log("The requested shader does not exist! Returning the phong shader instead.", logging::Severity::WARNING);
		return shader_programs_.at("Phong");
	}
}

void bind_shader(std::string_view name)
{
	const ShaderProgram& shader_program = get_shader(name);
	shader_program.use();
}

void insert_uniform(const GLchar* name)
{
	std::string name_str = std::string(name);
	if(!uniforms_.count(name_str))
	{
		GLint shader_program = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program);

		GLint location;
		if((location = glGetUniformLocation(shader_program, name)) == -1)
		{
			logging::log("The requested uniform variable (" + std::string(name) + ") does not exist!", logging::Severity::WARNING);
		}
		uniforms_.insert({name_str, location});
	}
}

void set_uniform_1f(const GLchar* name, GLfloat value)
{
	insert_uniform(name);
	glUniform1f(uniforms_.at(std::string(name)), value);
}

void set_uniform_1i(const GLchar* name, GLint value)
{
	insert_uniform(name);
	glUniform1i(uniforms_.at(std::string(name)), value);
}

void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value)
{
	insert_uniform(name);
	glUniformMatrix4fv(uniforms_.at(std::string(name)), 1, GL_FALSE, value);
}

void set_uniform_3f(const GLchar* name, glm::vec3 v)
{
	insert_uniform(name);
	glUniform3f(uniforms_.at(std::string(name)), v.x, v.y, v.z);
}

}