#include "skybox.h"
#include "Resource/resource_manager.h"

Skybox::Skybox(std::string_view directory_path)
	: mesh_id_(resource::add_skybox(directory_path))
{}

void Skybox::render() const
{
	if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		resource::bind_shader("Skybox");
		mesh->render();
	}
}