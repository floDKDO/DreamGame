#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <unordered_map>
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

		GLuint get_shader_program_id() const;
		void use() const;

		void insert_uniform(const GLchar* name);
		void set_uniform_1f(const GLchar* name, GLfloat value);
		void set_uniform_1i(const GLchar* name, GLint value);
		void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value);
		void set_uniform_3f(const GLchar* name, glm::vec3 v);
		void set_uniform_4f(const GLchar* name, glm::vec4 v);

	private:
		void create_shader(GLenum shader_type, std::string_view shader_path);
		void link() const;

		GLuint shader_program_id_;
		std::string shader_program_name_;
		std::vector<GLuint> shaders_;
		std::unordered_map<std::string, GLint> uniforms_;
};