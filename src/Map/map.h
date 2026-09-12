#pragma once

#include "Render/model.h"
#include "map_file.h"

#include <memory>

class Map
{
	public:
		explicit Map(std::string_view path);

		void draw();
		Model* get_model_by_name(std::string_view name);

	private:
		MapFile map_file_;

	public:
		std::vector<std::unique_ptr<Model>> models_; //TODO : remettre en private
		
};