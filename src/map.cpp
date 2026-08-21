#include "map.h"

Map::Map(std::string_view path)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{}

void Map::draw(ShaderProgram& shader_program)
{
	scene_->draw(shader_program);
}