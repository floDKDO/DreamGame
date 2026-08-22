#include "map.h"

Map::Map(std::string_view path)
	: map_file_(path), models_(map_file_.get_models())
{}

void Map::draw(ShaderProgram& shader_program)
{
	for(std::unique_ptr<Model>& model : models_)
	{
		model->draw(shader_program);
	}
	//scene_->draw(shader_program);
}