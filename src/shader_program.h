#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

class ShaderProgram
{
	public:
		ShaderProgram();
		~ShaderProgram();

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;

		void create_shader(GLenum shader_type, std::string_view shader_path);
		void link() const;
		void use() const;
		void set_uniform_1f(const GLchar* name, GLfloat value);
		void set_uniform_1i(const GLchar* name, GLint value);
		void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value);

	private:
		void insert_uniform(const GLchar* name);

		GLuint shader_program_;
		std::vector<GLuint> shaders_;
		std::unordered_map<std::string, GLint> uniforms_;
};