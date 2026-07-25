#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

class ShaderProgram
{
	public:
		ShaderProgram(std::vector<std::string> shader_paths);
		~ShaderProgram();

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& shader_program);
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shader_program);

		void use() const;
		void set_uniform_1f(const GLchar* name, GLfloat value);
		void set_uniform_1i(const GLchar* name, GLint value);
		void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value);
		void set_uniform_3f(const GLchar* name, glm::vec3 v);

	private:
		void create_shader(GLenum shader_type, std::string_view shader_path);
		void link() const;
		void insert_uniform(const GLchar* name);

		GLuint shader_program_;
		std::vector<GLuint> shaders_;
		std::unordered_map<std::string, GLint> uniforms_;
};