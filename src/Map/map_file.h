#pragma once

#include "Render/model.h"

#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MapFile
{
	public:
		explicit MapFile(std::string_view map_file_path);

		std::vector<std::unique_ptr<Model>> get_models() const;

	private:
		void open();
		void gltf_to_map_format();

		std::string map_file_path_;
		json gltf_data_;
		json map_data_;
};