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
	: sdl_(), window_(), glew_(glewInit()), shader_program_(), model_matrix_(glm::mat4(1.0f)), view_matrix_(glm::mat4(1.0f)), player_(view_matrix_),
	camera_(view_matrix_, player_.position_), running_(true), gamepad_(), temp_model_("resources/models/container.gltf")
{
	//model_matrix_ = glm::rotate(model_matrix_, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	int w, h;
	window_.get_size(&w, &h); 
	glViewport(0, 0, w, h);

	SDL_SetWindowRelativeMouseMode(window_.fetch(), true);

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
}

void Game::run()
{
	int w, h;
	window_.get_size(&w, &h);
	glm::mat4 projection_matrix = glm::mat4(1.0f);
	projection_matrix = glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 100.0f);

	shader_program_.create_shader(GL_VERTEX_SHADER, "resources/shaders/base_shader.vert");
	shader_program_.create_shader(GL_FRAGMENT_SHADER, "resources/shaders/base_shader.frag");
	shader_program_.link();
	shader_program_.use();
	shader_program_.set_uniform_1i("texture_sampler0_", 0);
	shader_program_.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(model_matrix_));
	shader_program_.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(view_matrix_));
	shader_program_.set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection_matrix));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while(running_)
	{
		handle_events();

		////////////////////////////////////////////////////////////////////////////////////////
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)
		////////////////////////////////////////////////////////////////////////////////////////

		update();
		draw();
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
		camera_.handle_events(e);
		player_.handle_events(e);

		////////////////////////////////////////////////////////////////////////////////////////
		ImGui_ImplSDL3_ProcessEvent(&e);
		////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_program_.use();
	//model_matrix_ = glm::rotate(model_matrix_, glm::radians(float(glm::sin(SDL_GetTicks() / 1000))), glm::vec3(1.0f, 0.0f, 0.0f));
	shader_program_.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(model_matrix_));
	shader_program_.set_uniform_matrix_4fv("view_matrix_", glm::value_ptr(view_matrix_));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	temp_model_.draw(shader_program_);
	player_.draw(shader_program_);

	////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////

	window_.swap_buffers();
}

void Game::check_gamepad()
{
	static Uint64 last_time = 0, current_time = 0;
	current_time = SDL_GetTicks();
	if(!gamepad_.is_open() && current_time > last_time + 1000) //tester une fois par seconde
	{
		gamepad_.open();
		last_time = current_time;
	}
}

void Game::update()
{
	camera_.update();
	player_.update();
	check_gamepad();
	shader_program_.set_uniform_3f("view_position_", camera_.camera_position_);
}