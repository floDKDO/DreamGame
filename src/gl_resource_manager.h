#pragma once

#include "Render/shader_program.h"
#include "Render/mesh.h"
#include "Render/texture.h"

#include <string_view>

namespace resource
{

//TODO : devrait sûrement être placé autre part
struct MeshKey
{
	std::string file_name_ = "";
	int32_t mesh_index_ = -1;
};

std::string add_texture(Texture texture);

//TODO : voir si je peux modifier l'implémentation pour quand même utiliser const
Texture* get_texture(std::string texture_key); //ne retourne pas de const Texture* car la fonction glCreateTextures() modifie son troisième paramètre

MeshKey add_mesh(std::string_view path, int32_t mesh_index, std::vector<GLushort> ebo_values, Vertices vertices, std::vector<std::string> texture_keys, GLenum draw_mode);
MeshKey add_mesh(std::string_view path, int32_t mesh_index, std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode);
const Mesh* get_mesh(MeshKey mesh_key);

void add_shader(std::string_view name, std::vector<std::string> shader_path);
const ShaderProgram& get_shader(std::string_view name);
void bind_shader(std::string_view name);

//TODO : ces méthodes devraient être placées autre part
void insert_uniform(const GLchar* name);
void set_uniform_1f(const GLchar* name, GLfloat value);
void set_uniform_1i(const GLchar* name, GLint value);
void set_uniform_matrix_4fv(const GLchar* name, const GLfloat* value);
void set_uniform_3f(const GLchar* name, glm::vec3 v);

}