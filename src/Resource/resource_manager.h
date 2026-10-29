#pragma once

#include "shader_program.h"
#include "Render/mesh.h"
#include "Render/texture.h"
#include "Common/transform.h"
#include "RAII_SDL3/font.h"
#include "RAII_SDL3/surface.h"

#include <string_view>
#include <unordered_map>

class Model;

namespace resource
{

void destroy_all_resources();

std::size_t add_texture(Texture texture);
Texture* get_texture(std::size_t id); //ne retourne pas de const Texture* car la fonction glCreateTextures() modifie son troisième paramètre

std::size_t add_mesh(std::string_view file_name, int32_t mesh_index, Mesh::Info mesh_info);
std::size_t add_2d_text(std::string_view text, const sdl::Surface& surface);
std::size_t add_2d_image(std::string_view path);
std::size_t add_skybox(std::string_view directory_path);
const Mesh* get_mesh(std::size_t id);

std::size_t add_aabb_mesh(std::string_view file_name, int32_t mesh_index, Mesh::Info aabb_mesh_info);
const Mesh* get_aabb_mesh(std::size_t id);

std::size_t add_model(std::string_view path, Transform transform);
std::size_t add_model(std::string_view path);
Model* get_model(std::size_t id); //ne retourne pas de const cat la classe Player a besoin de modifier le modèle du joueur
std::unordered_map<std::size_t, Model>& get_models();

void add_shader(std::string_view name, std::vector<std::string> shader_paths);
ShaderProgram* bind_shader(std::string_view name); //ne retourne pas de const car les méthodes d'ajout et de modification de variables uniformes ne sont pas const
ShaderProgram* get_currently_bound_shader();

std::size_t add_font(std::string_view font_path, float font_size);
sdl::Font* get_font(std::size_t id); //TODO : retourner une référence ?

}