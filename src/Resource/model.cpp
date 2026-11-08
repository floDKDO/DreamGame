#include "model.h"
#include "resource_manager.h"
#include "OpenAL/source.h"

#include <iostream>

//Attention : pour l'instant, si je construit un Model directement (sans passer par un MapFile), la transform indiquée dans le fichier glTF du Model ne sera pas prise en compte
//=> soit "Apply all transforms" sur Blender ou définir nous-même une Transform dans Model::Info
Model::Model(Info model_info)
	: model_info_(model_info), gltf_file_(model_info.path_), root_node_(gltf_file_.get_root_node()),
	ambient_sound_id_(model_info.ambient_sound_path_.empty() ? std::numeric_limits<std::size_t>::max() : resource::add_al_source(model_info.ambient_sound_path_, true))
{
	set_translation(model_info.transform_.position_);
	set_rotation(model_info.transform_.rotation_);
	set_scale(model_info.transform_.scale_);

	if(ambient_sound_id_ != std::numeric_limits<std::size_t>::max())
	{
		al::Source* source = resource::get_al_source(ambient_sound_id_);
		source->set_position(get_position());
		source->play();
	}
}

void Model::render()
{
	if(get_name() == "Light source") //TODO : hardcodé
	{
		if(ShaderProgram* shader_program = resource::bind_shader("Phong"); shader_program != nullptr)
		{
			shader_program->set_uniform_3f("light_position_", get_position());
		}
	}
	if(get_name() == "Light source.001") //TODO : hardcodé
	{
		if(ShaderProgram* shader_program = resource::bind_shader("Phong"); shader_program != nullptr)
		{
			shader_program->set_uniform_3f("light_position2_", get_position());
		}
	}
	if(get_name() == "Light source.002") //TODO : hardcodé
	{
		if(ShaderProgram* shader_program = resource::bind_shader("Phong"); shader_program != nullptr)
		{
			shader_program->set_uniform_3f("light_position3_", get_position());
		}
	}
	root_node_->render();
}

const glm::vec3& Model::get_position() const
{
	return root_node_->get_position();
}

std::string Model::get_name() const
{
	return model_info_.name_;
}

const gltf::Node* Model::get_root_node()
{
	return root_node_.get();
}

void Model::add_translation(glm::vec3 position)
{
	root_node_->add_translation(position);
}

void Model::add_translation_x(float x)
{
	root_node_->add_translation_x(x);
}

void Model::add_translation_y(float y)
{
	root_node_->add_translation_y(y);
}

void Model::add_translation_z(float z)
{
	root_node_->add_translation_z(z);
}

void Model::set_translation(glm::vec3 position)
{
	root_node_->set_translation(position);
}

void Model::add_rotation(glm::quat rotation)
{
	root_node_->add_rotation(rotation);
}

void Model::set_rotation(glm::quat rotation)
{
	root_node_->set_rotation(rotation);
}

void Model::add_scale(glm::vec3 scale)
{
	root_node_->add_scale(scale);
}

void Model::set_scale(glm::vec3 scale)
{
	root_node_->set_scale(scale);
}