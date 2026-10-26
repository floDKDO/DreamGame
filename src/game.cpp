#include "game.h"
#include "OpenAL/openal.h"
#include "Resource/resource_manager.h"

//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_sdl3.h>
//#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

Game::Game()
	: backend_(),
	player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	running_(true), test_map_("resources/maps/corridor.gltf"),
	gizmo_(nullptr), test_image_("resources/images/nakazato.jpg"),
	test_image_2_("resources/images/yuri_tea.png"),
	textbox_(input_manager_), skybox_("resources/images/skybox")
{
	std::size_t gizmo_model_id = resource::add_model("resources/models/axis_gizmo.glb");
	gizmo_ = resource::get_model(gizmo_model_id);
	textbox_.set_dialogues({{"Yuri", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."}, {"Unknown", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."}});
}

void Game::run()
{
	std::string temp_model_name("test"); //ici, "test" serait le nom du modèle
	audio::set_listener_position(player_.get_model()->get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());
	audio::set_listener_velocity(glm::vec3(0.0f));
	audio::create_source(temp_model_name, "resources/audio/test.wav"); 
	audio::set_source_gain(temp_model_name, 1.0f);
	audio::set_source_gain(temp_model_name, 1.0f);
	audio::set_source_pitch(temp_model_name, 1.0f);
	audio::set_source_reference_distance(temp_model_name, 1.0f);
	audio::set_source_max_distance(temp_model_name, 20.0f);
	audio::set_source_rolloff_factor(temp_model_name, 1.0f);
	audio::set_source_position(temp_model_name, glm::vec3(0.0f));
	//audio::play_source(temp_model_name);

	Uint64 begin_current_frame = 0; //en ms
	Uint64 last_frame = 0;          //en ms
	float  delta_time = 0;          //en secondes
	Uint64 last_fps_refresh = 0;    //en ms
	unsigned int frame_count_this_second = 0; //nombre de frames par seconde

	while(running_)
	{
		begin_current_frame = SDL_GetTicks();
		delta_time = (float(begin_current_frame) - float(last_frame)) / 1000.0f; //conversion en secondes

		handle_events();

		//TODO
		////////////////////////////////////////////////////////////////////////////////////////
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplSDL3_NewFrame();
		//ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)
		////////////////////////////////////////////////////////////////////////////////////////

		update(delta_time);

		backend_.begin_frame();
		render();
		backend_.end_frame();

		update_fps_count(last_fps_refresh, frame_count_this_second);
		last_frame = begin_current_frame;
	}

	audio::destroy_source(temp_model_name);
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_EVENT_KEY_DOWN:
				if(e.key.key == SDLK_ESCAPE)
				{
					running_ = false;
				}
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				running_ = false;
				break;

			default:
				break;
		}
		backend_.handle_events(e);
		input_manager_.handle_events(e);

		////////////////////////////////////////////////////////////////////////////////////////
		//ImGui_ImplSDL3_ProcessEvent(&e); //TODO
		////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Game::render()
{
	backend_.depth_write(false); //comme les depth values de la skybox sont sûrement plus petites que celles des modèles, on désactive l'écriture dans le depth buffer (il reste rempli de 1.0 donc les modèles pourront s'afficher)
	resource::bind_shader("Skybox");
	skybox_.render();
	backend_.depth_write(true);

	//resource::bind_shader("2d");
	//textbox_.render();
	//test_image_.render();
	//test_image_2_.render();

	/*resource::bind_shader("Phong");
	for(auto& [model_id, model] : resource::get_models())
	{
		model.render();
	}*/

	audio::set_listener_position(player_.get_model()->get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());

	//TODO
	////////////////////////////////////////////////////////////////////////////////////////
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////
}

void Game::update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const
{
	frame_count_this_second += 1;
	if(SDL_GetTicks() >= last_fps_refresh + 1000) //tester une fois par seconde pour obtenir des frames par seconde
	{
		backend_.update_window_fps(frame_count_this_second);
		last_fps_refresh = SDL_GetTicks();
		frame_count_this_second = 0;
	}
}

void Game::update(float delta_time)
{
	camera_.update(delta_time);
	player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	input_manager_.update(delta_time);
	textbox_.update(delta_time);
}