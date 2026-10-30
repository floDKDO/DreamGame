#include "cubemap.h"
#include "Resource/resource_manager.h"

Cubemap::Cubemap(std::string_view directory_path)
	: mesh_id_(resource::add_cubemap(directory_path))
{}

void Cubemap::render() const
{
	if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
	{
		resource::bind_shader("Skybox");
		mesh->render();
	}
}