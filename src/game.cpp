#include "game.h"
#include "OpenAL/openal.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : free ImGui
//TODO : créer des constantes pour les valeurs par défaut de translation, rotation et scale
//TODO : voir si je renomme position en translation

Game::Game()
	: backend_(), //window_(),
	player_(input_manager_),
	camera_(input_manager_, player_.model_.get_position()), 
	running_(true), gamepad_(), test_map_("resources/maps/corridor.gltf"), 
	gizmo_("resources/models/axis_gizmo.glb"),
	fov_(glm::radians(45.0f)), near_plane_(0.1f), far_plane_(100.0f), perspective_projection_matrix_(1.0f)
{
	shader_programs_.insert(std::make_pair("Base", ShaderProgram("Base", {"resources/shaders/base_shader.vert", "resources/shaders/base_shader.frag"})));
	shader_programs_.insert(std::make_pair("Phong", ShaderProgram("Phong", {"resources/shaders/base_shader.vert", "resources/shaders/phong_shader.frag"})));
}

void Game::run()
{
	int w, h;
	backend_.get_window_size(&w, &h);
	perspective_projection_matrix_ = glm::perspective(fov_, float(w) / float(h), near_plane_, far_plane_);

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_1i("texture_sampler0_", 0);
	phong_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	phong_program.set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(perspective_projection_matrix_));

	//TODO
	/*ShaderProgram& base_program = shader_programs_.at("Base");
	base_program.use();
	base_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	base_program.set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection_matrix));*/

	std::string temp_model_name("test"); //ici, "test" serait le nom du modèle
	audio::set_listener_position(player_.model_.get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());
	audio::set_listener_velocity(glm::vec3(0.0f));
	audio::create_source(temp_model_name, "resources/test.wav"); 
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
		draw();

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
			case SDL_EVENT_WINDOW_RESIZED:
				glViewport(0, 0, e.window.data1, e.window.data2);
				break;

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
		input_manager_.handle_events(e);

		////////////////////////////////////////////////////////////////////////////////////////
		//ImGui_ImplSDL3_ProcessEvent(&e); //TODO
		////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO : à gérer
	//ShaderProgram& base_program = shader_programs_.at("Base");
	//base_program.use();
	//base_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	phong_program.set_uniform_3f("view_position_", camera_.camera_position_);
	phong_program.set_uniform_3f("light_position_", test_map_.get_model_by_name("Light source")->get_position());

	player_.draw(phong_program);
	gizmo_.draw(phong_program);
	test_map_.draw(phong_program);

	audio::set_listener_position(player_.model_.get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());

	//TODO
	////////////////////////////////////////////////////////////////////////////////////////
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////

	backend_.swap_window_buffers(); //TODO : pas ouf, devrait directement être dans la classe Backend
}

void Game::update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const
{
	frame_count_this_second += 1;
	if(SDL_GetTicks() >= last_fps_refresh + 1000) //tester une fois par seconde pour obtenir des frames par seconde
	{
		//window_.update_fps(frame_count_this_second); //TODO
		last_fps_refresh = SDL_GetTicks();
		frame_count_this_second = 0;
	}
}

void Game::update(float delta_time)
{
	camera_.update(delta_time);
	player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	gamepad_.check(1000); //tester une fois par seconde
	input_manager_.update(delta_time);
}