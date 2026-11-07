#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

using json = nlohmann::json;

class MapFile
{
	public:
		explicit MapFile(std::string_view map_file_path);

		std::string get_map_name() const;
		std::string get_map_music() const;

	private:
		void open();
		void gltf_to_map_format();
		void add_models() const;

		std::string map_file_path_;
		json gltf_data_;
		json map_data_;
};