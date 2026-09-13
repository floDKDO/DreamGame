#include "gl_resource_manager.h"
#include "Logging/logging.h"

#include <map>
#include <string>
#include <iostream>

namespace resource
{

//TODO : devrait sûrement être placé autre part
bool operator<(const MeshKey& a, const MeshKey& b)
{
	if(a.file_name_ < b.file_name_)
	{
		return true;
	}
	else if(b.file_name_ < a.file_name_)
	{
		return false;
	}
	else if(a.mesh_index_ < b.mesh_index_)
	{
		return true;
	}
	else
	{
		return false;
	}

	//TODO : ne marche pas pour ce cas-là : 
	//a = "aaa", 42 ; b = "zzz", 1
	//a < b => true
	//b < a => true
	//return (a.file_name_ < b.file_name_) || (a.mesh_index_ < b.mesh_index_);
}

//TODO : avant optimisations, 104 Meshes sont créés. Après optimisations, 15 Meshes sont créés

std::map<MeshKey, Mesh> meshes_; //TODO : ne marche pas avec une std::unordered_map
std::unordered_map<std::string, ShaderProgram> shader_programs_;
std::unordered_map<std::string, GLint> uniforms_;

void add_mesh(std::string_view path, int32_t mesh_index, std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture> textures, GLenum draw_mode)
{
	MeshKey mesh_key = {std::string(path), mesh_index};
	meshes_.insert(std::make_pair(mesh_key, Mesh(ebo_values, vertices, textures, draw_mode)));
}

void add_mesh(std::string_view path, int32_t mesh_index, std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode)
{
	MeshKey mesh_key = {std::string(path), mesh_index};
	meshes_.insert(std::make_pair(mesh_key, Mesh(ebo_values, vertices, draw_mode)));
}

const Mesh* get_mesh(MeshKey mesh_key)
{
	if(meshes_.count(mesh_key))
	{
		return &meshes_.at(mesh_key);
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