#include "skybox.h"
#include "Resource/resource_manager.h"

Skybox::Skybox(std::string_view directory_path)
	: back_(std::string(directory_path) + "/back.bmp"), bottom_(std::string(directory_path) + "/bottom.bmp"), front_(std::string(directory_path) + "/front.bmp"),
	left_(std::string(directory_path) + "/left.bmp"), right_(std::string(directory_path) + "/right.bmp"), top_(std::string(directory_path) + "/top.bmp"),
	mesh_id_(resource::add_skybox(directory_path, back_, bottom_, front_, left_, right_, top_))
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