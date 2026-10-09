#pragma once

#include "Resource/shader_program.h"
#include "Render/mesh.h"
#include "Render/texture.h"
#include "Render/text.h"
#include "Common/transform.h"
#include "RAII_SDL3/font.h"

#include <string_view>
#include <unordered_map>

class Model;

namespace resource
{

void destroy_all_resources();

std::string add_texture(Texture texture);
Texture* get_texture(std::string_view texture_key); //ne retourne pas de const Texture* car la fonction glCreateTextures() modifie son troisième paramètre

Mesh::Id add_mesh(Mesh::Id mesh_id, Mesh::Info mesh_info);
const Mesh* get_mesh(Mesh::Id mesh_id);

Mesh::Id add_aabb_mesh(Mesh::Id aabb_mesh_id, Mesh::Info aabb_mesh_info);
const Mesh* get_aabb_mesh(Mesh::Id aabb_mesh_id);

std::size_t add_model(std::string_view path, Transform transform);
std::size_t add_model(std::string_view path);
Model* get_model(std::size_t model_id); //ne retourne pas de const cat la classe Player a besoin de modifier le modèle du joueur
std::unordered_map<std::size_t, Model>& get_models();

void add_shader(std::string_view name, std::vector<std::string> shader_paths);
ShaderProgram* bind_shader(std::string_view name); //ne retourne pas de const car les méthodes d'ajout et de modification de variables uniformes ne sont pas const
ShaderProgram* get_currently_bound_shader();

Text::FontId add_font(std::string_view font_path, float font_size);
sdl::Font* get_font(std::string_view font_path, float font_size);

}