#include "shader_program.h"

#include <fstream>
#include <sstream>
#include <iostream>


namespace
{

constexpr int info_log_size_ = 512;

}

ShaderProgram::ShaderProgram()
	: shader_program_(glCreateProgram())
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shader_program_);
}

void ShaderProgram::create_shader(GLenum shader_type, std::string_view shader_path)
{
	shaders_.push_back(glCreateShader(shader_type));
	GLuint shader = shaders_.back();

	std::string shader_path_str = std::string(shader_path);
	std::ifstream shader_file(shader_path_str);
	if(!shader_file.is_open())
	{
		std::cerr << "Error: unable to open the requested file (" << shader_path << ")!\n";
	}

	std::stringstream shader_stream;
	shader_stream << shader_file.rdbuf();

	std::string str_source_code = shader_stream.str();
	const char* source_code = str_source_code.c_str();

	glShaderSource(shader, 1, &source_code, nullptr);
	glCompileShader(shader);

	GLint compile_status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if(!compile_status)
	{
		GLchar info_log[info_log_size_];
		glGetShaderInfoLog(shader, info_log_size_, nullptr, info_log);
		std::cerr << "Error: (Shader) " << info_log << std::endl;
	}
}

void ShaderProgram::link() const
{
	for(GLuint shader : shaders_)
	{
		glAttachShader(shader_program_, shader);
	}
	glLinkProgram(shader_program_);

	GLint link_status;
	glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
	if(!link_status)
	{
		GLchar info_log[info_log_size_];
		glGetProgramInfoLog(shader_program_, info_log_size_, nullptr, info_log);
		std::cerr << "Error: (program shader) " << info_log << std::endl;
	}

	for(GLuint shader : shaders_)
	{
		glDeleteShader(shader);
	}
}

void ShaderProgram::use() const
{
	glUseProgram(shader_program_);
}

void ShaderProgram::insert_uniform(const GLchar* name)
{
	std::string name_str = std::string(name);
	if(!uniforms_.count(name_str))
	{
		GLint location;
		if((location = glGetUniformLocation(shader_program_, name)) == -1)
		{
			std::cerr << "Error: the requested uniform variable (" << name << ") does not exist!\n";
		}
		uniforms_.insert({name_str, location});
	}
}

void ShaderProgram::set_uniform_1f(const GLchar* name, GLfloat value)
{
	insert_uniform(name);
	glUniform1f(uniforms_.at(std::string(name)), value);
}

void ShaderProgram::set_uniform_1i(const GLchar* name, GLint value)
{
	insert_uniform(name);
	glUniform1i(uniforms_.at(std::string(name)), value);
}


void ShaderProgram::set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value)
{
	insert_uniform(name);
	glUniformMatrix4fv(uniforms_.at(std::string(name)), 1, GL_FALSE, value);
}