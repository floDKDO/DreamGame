#pragma once

#include "node.h"

namespace gltf
{

class Model //<=> Root node
{
	public:
		Model();

	private:
		//TODO : Mettre tous les noeuds qui sont en relation (children)
		std::vector<std::pair<unsigned int, Node>> nodes_; //unsigned int = node level
};

}