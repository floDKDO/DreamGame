#include "scene.h"

#include <iostream>

Scene::Scene(std::string_view path)
	: path_(path)
{
	/*
	///// open_gltf_file() //////////////////////////////////////////////////////////////////////////////////////////
	tg3_model model;
	tg3_error_stack errors;
	tg3_parse_options opts;
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors);

	tg3_error_code err = tg3_parse_file(&model, &errors, path_.c_str(), uint32_t(path_.length()), &opts);
	if(err != TG3_OK)
	{
		for(uint32_t i = 0; i < errors.count; i++)
		{
			std::string str = errors.entries[i].message ? errors.entries[i].message : "(null)";
			std::cout << int(errors.entries[i].severity) << ", " << str << std::endl;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for(uint32_t i = 0; i < model.nodes_count; ++i)
	{
		tg3_node node = model.nodes[i];

	}

	for(Node& node : nodes_)
	{
		node.
	}*/
}

Scene::~Scene()
{

}

void Scene::draw(ShaderProgram& shader_program)
{
	for(Node& node : nodes_)
	{
		node.draw(shader_program);
	}
}