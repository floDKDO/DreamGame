#include "in_game_state.h"
#include "Resource/resource_manager.h"
#include "state_machine.h"

InGameState::InGameState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend), player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	test_map_("resources/maps/corridor.gltf"), gizmo_(nullptr),
	test_image_("resources/images/yuri_tea.png"),
	textbox_(input_manager), skybox_("resources/images/skybox")
{
	std::size_t gizmo_model_id = resource::add_model("resources/models/axis_gizmo.glb");
	gizmo_ = resource::get_model(gizmo_model_id);
	textbox_.set_dialogues({{"Yuri", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."}, {"Unknown", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."}});
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

	if(input_manager_.get_input_info().x_movement_intensity_ > 0.2f)
	{
		push_state(StateId::TITLE_SCREEN);
	}
}