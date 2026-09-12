#pragma once

#include "Render/shader_program.h"

#include <string_view>

namespace resource
{

void add_shader(std::string_view name, std::vector<std::string> shader_path);
const ShaderProgram& get_shader(std::string_view name);
void bind_shader(std::string_view name);

void insert_uniform(const GLchar* name);
void set_uniform_1f(const GLchar* name, GLfloat value);
void set_uniform_1i(const GLchar* name, GLint value);
void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value);
void set_uniform_3f(const GLchar* name, glm::vec3 v);

}