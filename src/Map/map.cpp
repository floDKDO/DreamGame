//TODO : classe inutile pour l'instant

//#include "map.h"
//
//Map::Map(std::string_view path)
//	: map_file_(path), models_(map_file_.get_models())
//{}
//
//void Map::render()
//{
//	for(const std::unique_ptr<Model>& model : models_)
//	{
//		model->render();
//	}
//}
//
//const std::vector<std::unique_ptr<Model>>& Map::get_models() const
//{
//	return models_;
//}