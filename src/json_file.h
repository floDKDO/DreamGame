#pragma once

#include "model.h"

#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JSONFile
{
	public:
		explicit JSONFile(std::string_view gltf_json_file_path);

		std::vector<std::unique_ptr<Model>> get_models() const;

	private:
		void open();
		void gltf_to_map_format();

		std::string gltf_json_file_path_;
		json gltf_data_;
		json map_data_;
};