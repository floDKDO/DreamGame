#pragma once

#include "Render/shader_program.h"
#include "Render/mesh.h"
#include "Render/texture.h"
#include "Common/transform.h"

#include <string_view>

class Model;

namespace resource
{

std::string add_texture(Texture texture);

//TODO : voir si je peux modifier l'implémentation pour quand même utiliser const
Texture* get_texture(std::string texture_key); //ne retourne pas de const Texture* car la fonction glCreateTextures() modifie son troisième paramètre

Mesh::MeshId add_mesh(Mesh::MeshId mesh_id, Mesh::MeshInfo mesh_info);
const Mesh* get_mesh(Mesh::MeshId mesh_id);

Mesh::MeshId add_aabb_mesh(Mesh::MeshId aabb_mesh_id, Mesh::MeshInfo aabb_mesh_info);
const Mesh* get_aabb_mesh(Mesh::MeshId aabb_mesh_id);

std::string add_model(std::string_view path, Transform transform);
std::string add_model(std::string_view path);
Model* get_model(std::string_view name); //Ne retourne pas de const cat la classe Player a besoin de modifier le modèle du joueur

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