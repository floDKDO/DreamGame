#include "shader_program.h"
#include "Common/utils.h"
#include "Logging/logging.h"

#include <filesystem>
#include <iostream>


namespace
{

constexpr int info_log_size_ = 512;

}

ShaderProgram::ShaderProgram(std::string_view shader_program_name, std::vector<std::string> shader_paths)
	: shader_program_(glCreateProgram()), shader_program_name_(shader_program_name)
{
	for(const std::string& shader_path : shader_paths)
	{
		std::filesystem::path path = shader_path;

		if(path.extension() == ".vert")
		{
			create_shader(GL_VERTEX_SHADER, shader_path);
		}
		else if(path.extension() == ".frag")
		{
			create_shader(GL_FRAGMENT_SHADER, shader_path);
		}
	}
	link();
}

ShaderProgram::ShaderProgram(ShaderProgram&& shader_program)
	: shader_program_(shader_program.shader_program_), shader_program_name_(shader_program.shader_program_name_), shaders_(shader_program.shaders_)
{
	shader_program.shader_program_ = 0; // void glDeleteProgram(GLuint program); -> "A value of 0 for program will be silently ignored."
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader_program)
{
	if(this == &shader_program)
	{
		return *this;
	}

	if(glIsProgram(shader_program_) == GL_TRUE)
	{
		glDeleteProgram(shader_program_);
	}

	shader_program_ = shader_program.shader_program_;
	shader_program.shader_program_ = 0; // void glDeleteProgram(GLuint program); -> "A value of 0 for program will be silently ignored."
	return *this;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shader_program_);
}

void ShaderProgram::create_shader(GLenum shader_type, std::string_view shader_path)
{
	shaders_.push_back(glCreateShader(shader_type));
	GLuint shader = shaders_.back();

	std::string str_source_code = utils::get_string_from_file(shader_path);
	const char* source_code = str_source_code.c_str();

	glShaderSource(shader, 1, &source_code, nullptr);
	glCompileShader(shader);

	GLint compile_status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if(!compile_status)
	{
		GLchar info_log[info_log_size_];
		glGetShaderInfoLog(shader, info_log_size_, nullptr, info_log);
		logging::log("Shader error: " + std::string(info_log), logging::Severity::WARNING);
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
		logging::log("Shader linker error: " + std::string(info_log), logging::Severity::WARNING);
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