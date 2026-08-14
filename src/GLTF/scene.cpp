#include "scene.h"

#include <iostream>

namespace gltf
{

Scene::Scene(std::string_view path)
	: gltf_file_(path), path_(path), models_(gltf_file_.get_models())
{}

Scene::~Scene()
{

}

void Scene::draw(ShaderProgram& shader_program)
{
	for(Model& model : models_)
	{
		model.draw(shader_program);
	}
}

Model& Scene::get_model_by_name(std::string name)
{
	for(Model& model : models_)
	{
		if(model.get_name() == name)
		{
			return model;
		}
	}
	//TODO : à gérer
}

}