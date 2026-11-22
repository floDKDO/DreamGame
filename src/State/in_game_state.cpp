#include "in_game_state.h"
#include "Resource/resource_manager.h"
#include "OpenAL/listener.h"
#include "Common/constants.h"
#include "Common/utils.h"

InGameState::InGameState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend), player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	test_map_(std::string(constants::maps_path_) + "corridor_multiple_lights.gltf"), gizmo_(nullptr),
	test_image_(std::string(constants::images_path_) + "yuri_tea.png"),
	textbox_(input_manager), skybox_(std::string(constants::cubemaps_path_) + "skybox/")
	//,fbo_(backend, constants::initial_window_width_ / 2, constants::initial_window_height_ / 2)
{
	std::size_t gizmo_model_id = resource::add_model(Model::Info{"Axis guizmo", std::string(constants::models_path_) + "axis_gizmo.glb"});
	gizmo_ = resource::get_model(gizmo_model_id);
	textbox_.set_dialogues({{"Yuri", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."}, {"Unknown", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."}});

	/*std::size_t map_music_id = resource::add_al_source(std::string(constants::musics_path_) + test_map_.get_map_music(), true);
	al::Source* music_source = resource::get_al_source(map_music_id);
	music_source->play();*/

	mesh_id_ = resource::add_mesh(Mesh::Info{utils::get_screen_ebo_values(), utils::get_screen_vertices(), {}});
	resource::bind_shader("Alpha");
	resource::get_currently_bound_shader()->set_uniform_1f("alpha_", 0.0f);
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
	//fbo_.bind();

	backend_.depth_write(false); //comme les depth values de la skybox sont sûrement plus petites que celles des modèles, on désactive l'écriture dans le depth buffer (il reste rempli de 1.0 donc les modèles pourront s'afficher)
	resource::bind_shader("Skybox");
	skybox_.render();
	backend_.depth_write(true);

	resource::bind_shader("2d");
	textbox_.render();
	test_image_.render();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	resource::bind_shader("Phong");
	for(auto& [model_id, model] : resource::get_models())
	{
		model.render();
	}
	glDisable(GL_CULL_FACE);

	//fbo_.render();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	static float alpha = 0.0f;
	static Uint64 then = SDL_GetTicks() + 5000;
	float div = std::clamp(float(SDL_GetTicks()) / float(then), 0.0f, 1.0f);
	resource::bind_shader("Alpha");
	resource::get_currently_bound_shader()->set_uniform_1f("alpha_", alpha);
	resource::get_mesh(mesh_id_)->render();
	alpha = utils::lerp(0.0f, 1.0f, div);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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