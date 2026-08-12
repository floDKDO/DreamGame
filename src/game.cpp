#include "game.h"
#include "utils.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : free ImGui

Game::Game()
	: sdl_(), window_(), glew_(glewInit()), player_(intput_manager_),
	camera_(intput_manager_, player_.model_.position_), running_(true), gamepad_(), temp_model_("resources/models/corridor.glb"), light_source_("resources/models/light_source.glb")
{
	int w, h;
	window_.get_size(&w, &h); 
	glViewport(0, 0, w, h);

	window_.set_relative_mouse_mode(true);

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(utils::message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE); //désactiver les messages de sévérité "Notification"

	////////////////////////////////////////////////////////////////////////////////////////
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.MouseDrawCursor = true; //afficher un curseur de souris même s'il est caché par SDL

	ImGui_ImplSDL3_InitForOpenGL(window_.fetch(), window_.get_context());
	ImGui_ImplOpenGL3_Init();
	////////////////////////////////////////////////////////////////////////////////////////

	shader_programs_.insert(std::make_pair("Base", ShaderProgram("Base", {"resources/shaders/base_shader.vert", "resources/shaders/base_shader.frag"})));
	shader_programs_.insert(std::make_pair("Phong", ShaderProgram("Phong", {"resources/shaders/base_shader.vert", "resources/shaders/phong_shader.frag"})));
}

void Game::run()
{
	int w, h;
	window_.get_size(&w, &h);
	glm::mat4 projection_matrix(1.0f);
	projection_matrix = glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 100.0f);

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_1i("texture_sampler0_", 0);
	phong_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	phong_program.set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection_matrix));

	ShaderProgram& base_program = shader_programs_.at("Base");
	base_program.use();
	base_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	base_program.set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection_matrix));

	light_source_.position_ = glm::vec3(0.0f, 5.0f, 0.0f);

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

		////////////////////////////////////////////////////////////////////////////////////////
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)
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
		intput_manager_.handle_events(e);

		////////////////////////////////////////////////////////////////////////////////////////
		ImGui_ImplSDL3_ProcessEvent(&e);
		////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderProgram& base_program = shader_programs_.at("Base");
	base_program.use();
	base_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	light_source_.draw(base_program);

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.use();
	phong_program.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(camera_.get_view_matrix()));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	temp_model_.draw(phong_program);
	player_.draw(phong_program);

	////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////

	window_.swap_buffers();
}

void Game::update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const
{
	frame_count_this_second += 1;
	if(SDL_GetTicks() >= last_fps_refresh + 1000) //tester une fois par seconde pour obtenir des frames par seconde
	{
		window_.update_fps(frame_count_this_second);
		last_fps_refresh = SDL_GetTicks();
		frame_count_this_second = 0;
	}
}

void Game::update(float delta_time)
{
	camera_.update(delta_time);
	player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	gamepad_.check(1000); //tester une fois par seconde
	intput_manager_.update(delta_time);

	ShaderProgram& phong_program = shader_programs_.at("Phong");
	phong_program.set_uniform_3f("view_position_", camera_.camera_position_);
	phong_program.set_uniform_3f("light_position_", light_source_.position_);
}