#include "map_file.h"
//#include "Logging/logging.h"

#include <fstream>
#include <filesystem>
#include <iostream>


MapFile::MapFile(std::string_view map_file_path)
	: map_file_path_(map_file_path)
{
	open();
	gltf_to_map_format();
	add_models();
}

void MapFile::add_models() const
{
	for(const auto& model : map_data_["models"].items())
	{
		json model_value = model.value();

		json translation = model_value["translation"];
		glm::vec3 translation_vec(translation[0], translation[1], translation[2]);

		json rotation = model_value["rotation"];
		glm::quat rotation_quat(rotation[3], rotation[0], rotation[1], rotation[2]);

		json scale = model_value["scale"];
		glm::vec3 scale_vec(scale[0], scale[1], scale[2]);

		Transform transform;
		transform.position_ = translation_vec;
		transform.rotation_ = rotation_quat;
		transform.scale_ = scale_vec;

		std::string model_filename = model_value["modelFilename"];

		resource::add_model("resources/models/" + model_filename, transform);
	}
}

void MapFile::open()
{
	std::filesystem::path path(map_file_path_);
	std::ifstream input_file(path);

	if(input_file.is_open())
	{
		gltf_data_ = json::parse(input_file, nullptr, false);

		if(gltf_data_.is_discarded())
		{
			gltf_data_ = json::object();
		}
	}
	input_file.close();
}

void MapFile::gltf_to_map_format()
{
	//copie des indices des root nodes dans un std::vector
	std::vector<int> root_node_indexes;
	json& scene = gltf_data_["scenes"][0];
	json& root_nodes_index = scene["nodes"];
	for(auto it = root_nodes_index.begin(); it != root_nodes_index.end(); ++it)
	{
		root_node_indexes.push_back(*it);
	}

	map_data_["models"] = {};
	map_data_["mapName"] = scene["name"];
	
	int j = 0;
	json& models = map_data_["models"];
	json& nodes = gltf_data_["nodes"];
	for(int i : root_node_indexes)
	{
		json& node = nodes[i];

		json model;
		model["instanceName"] = node["name"];
		model["modelFilename"] = node["extras"]["modelName"];

		if(node["translation"].is_null())
		{
			model["translation"] = json::array({0, 0, 0});
		}
		else
		{
			model["translation"] = node["translation"];
		}

		if(node["rotation"].is_null())
		{
			model["rotation"] = json::array({0, 0, 0, 1});
		}
		else
		{
			model["rotation"] = node["rotation"];
		}

		if(node["scale"].is_null())
		{
			model["scale"] = json::array({1, 1, 1});
		}
		else
		{
			model["scale"] = node["scale"];
		}

		models[j] = model;
		j += 1;
	}

	//logging::log(map_data_.dump(2), logging::Severity::DEBUG);

	//std::string output_filename = path.stem().string() + ".map";
	//std::ofstream output_file(output_filename);
	//output_file << output.dump(2);
	//output_file.close();
}