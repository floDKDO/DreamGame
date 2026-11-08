#include "in_game_state.h"
#include "Resource/resource_manager.h"
#include "OpenAL/listener.h"

InGameState::InGameState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend), player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	test_map_("resources/maps/corridor_multiple_lights.gltf"), gizmo_(nullptr),
	test_image_("resources/images/yuri_tea.png"),
	textbox_(input_manager), skybox_("resources/images/skybox")
{
	std::size_t gizmo_model_id = resource::add_model(Model::Info{"Axis guizmo", "resources/models/axis_gizmo.glb"});
	gizmo_ = resource::get_model(gizmo_model_id);
	textbox_.set_dialogues({{"Yuri", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."}, {"Unknown", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."}});
	
	std::size_t map_music_id = resource::add_al_source("resources/audio/musics/" + test_map_.get_map_music(), true);
	al::Source* music_source = resource::get_al_source(map_music_id);
	music_source->play();
}

void InGameState::on_show()
{
	//TODO : fondu
	std::cout << "ON SHOW! (in game)\n";
}

void InGameState::on_hide()
{
	//TODO : fondu
	std::cout << "ON HIDE! (in game)\n";
}

void InGameState::display()
{
	backend_.depth_write(false); //comme les depth values de la skybox sont sûrement plus petites que celles des modèles, on désactive l'écriture dans le depth buffer (il reste rempli de 1.0 donc les modèles pourront s'afficher)
	resource::bind_shader("Skybox");
	skybox_.render();
	backend_.depth_write(true);

	resource::bind_shader("2d");
	textbox_.render();
	test_image_.render();

	resource::bind_shader("Phong");
	for(auto& [model_id, model] : resource::get_models())
	{
		model.render();
	}
}

void InGameState::update([[maybe_unused]] float delta_time)
{
	camera_.update(delta_time);
	player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	textbox_.update(delta_time);

	al::set_listener_position(player_.get_model()->get_position());
	al::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());
	al::set_listener_velocity(glm::vec3(0.0f));
}