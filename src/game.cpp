#include "game.h"
#include "utils.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glTF/gltf.h"
#include "json_file.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : free ImGui
//TODO : créer des constantes pour les valeurs par défaut de translation, rotation et scale

Game::Game()
	: backend_(), //window_(),
	temp_(0.0f),  //TODO : à retirer (remplacer par la position du joueur)
	// //TODO : à gérer player_(input_manager_),
	camera_(input_manager_, temp_ /* //TODO : à gérer player_.model_.transform_.position_*/), 
	running_(true), gamepad_(), test_map_("resources/maps/corridor_v3.gltf"), 
	/*test_scene_("resources/models/corridor.gltf"),*/ gizmo_("resources/models/axis_gizmo.glb"), 
	perspective_projection_matrix_(1.0f) //,temp_model_("resources/models/corridor.glb"), light_source_("resources/models/light_source.glb")
{
	shader_programs_.insert(std::make_pair("Base", ShaderProgram("Base", {"resources/shaders/base_shader.vert", "resources/shaders/base_shader.frag"})));
	shader_programs_.insert(std::make_pair("Phong", ShaderProgram("Phong", {"resources/shaders/base_shader.vert", "resources/shaders/phong_shader.frag"})));

	///
	JSONFile map_level("resources/maps/corridor_v2.gltf");
	///
}

void Game::run()
{
	int w, h;
	backend_.get_window_size(&w, &h);
	perspective_projection_matrix_ = glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 100.0f); //TODO : hardcodé et voir si je garde ces valeurs

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

	//TODO : à gérer
	//light_source_.transform_.position_ = glm::vec3(0.0f, 5.0f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	//TODO
	//ShaderProgram& base_program = shader_programs_.at("Base");
	//base_program.use();
	//base_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	//TODO : à gérer
	//light_source_.draw(base_program);

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	phong_program.set_uniform_3f("view_position_", camera_.camera_position_);
	//TODO : à gérer
	//temp_model_.draw(phong_program);
	//player_.draw(phong_program);
	//test_scene_.draw(phong_program);
	gizmo_.draw(phong_program);
	test_map_.draw(phong_program);

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
	//player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	gamepad_.check(1000); //tester une fois par seconde
	input_manager_.update(delta_time);

	//TODO : à gérer (mettre dans draw())
	//phong_program.set_uniform_3f("light_position_", light_source_.transform_.position_);

	//TODO : à supprimer
	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_3f("light_position_", glm::vec3(0.0f, 4.0f, 0.0f));
}