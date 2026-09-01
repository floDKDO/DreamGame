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
}

Model* Map::get_model_by_name(std::string_view name)
{
	for(std::unique_ptr<Model>& model : models_)
	{
		if(model->get_name() == name)
		{
			return model.get();
		}
	}
	return nullptr;
}