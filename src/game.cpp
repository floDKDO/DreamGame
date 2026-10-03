#include "game.h"
#include "OpenAL/openal.h"
#include "Resource/gl_resource_manager.h"
#include "Projection/projection.h"

//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_sdl3.h>
//#include <imgui/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game::Game()
	: backend_(),
	player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	running_(true), test_map_("resources/maps/corridor.gltf"),
	gizmo_(nullptr), test_image_("resources/images/yuri_tea.png", glm::vec2(500.0f, 50.0f), glm::vec2(200.0f), 90.0f),
	test_image_2_("resources/images/yuri_tea.png", glm::vec2(800.0f, 300.0f), glm::vec2(200.0f), 0.0f),
	test_text_("C'est un texte de test !", {0, 255, 255, 255}),
	test_text_2_("INITIAL D !", {255, 255, 255, 255}, glm::vec2(200.0f))
{
	std::size_t gizmo_model_id = resource::add_model("resources/models/axis_gizmo.glb");
	gizmo_ = resource::get_model(gizmo_model_id);
	test_text_2_.set_scale(glm::vec2(5.0f));
}

void Game::run()
{
	glm::ivec2 window_size = backend_.get_window_size();

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
		render();

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 window_size = backend_.get_window_size();

	/*if(ShaderProgram* shader_program_2d = resource::bind_shader("2d"); shader_program_2d != nullptr)
	{
		shader_program_2d->set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_orthographic_matrix(float(window_size.x), float(window_size.y))));
	}
	test_image_.render();
	test_image_2_.render();*/

	/*if(ShaderProgram* shader_program_text = resource::bind_shader("Text"); shader_program_text != nullptr)
	{
		shader_program_text->set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_orthographic_matrix(float(window_size.x), float(window_size.y))));
	}
	test_text_.render();
	test_text_2_.render();*/

	if(ShaderProgram* shader_program_phong = resource::bind_shader("Phong"); shader_program_phong != nullptr)
	{
		shader_program_phong->set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_perspective_matrix(float(window_size.x) / float(window_size.y))));
	}
	for(auto& [model_id, model] : resource::get_models())
	{
		model.render();
	}

	audio::set_listener_position(player_.get_model()->get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());

	//TODO
	////////////////////////////////////////////////////////////////////////////////////////
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////

	backend_.swap_window_buffers();
	backend_.reset_opengl_states();
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
}