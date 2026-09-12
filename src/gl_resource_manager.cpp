#include "gl_resource_manager.h"
#include "Logging/logging.h"

#include <unordered_map>
#include <string>
#include <iostream>

namespace resource
{

std::unordered_map<std::string, ShaderProgram> shader_programs_;
std::unordered_map<std::string, GLint> uniforms_;

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
	std::cout << "Voici: " << name << std::endl;
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