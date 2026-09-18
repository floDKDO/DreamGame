#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <string_view>

class ShaderProgram
{
	public:
		ShaderProgram(std::string_view shader_program_name, std::vector<std::string> shader_paths);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& shader_program);
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shader_program);
		~ShaderProgram();

		void use() const;

	private:
		void create_shader(GLenum shader_type, std::string_view shader_path);
		void link() const;

		GLuint shader_program_;
		std::string shader_program_name_;
		std::vector<GLuint> shaders_;
};