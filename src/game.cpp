#include "game.h"
#include "gl/glew.h"
#include "stb/stb_image.h"

#include <iostream>

Game::Game()
	: sdl_(), window_(), glew_(glewInit()), shader_program_(), 
	camera_position_(glm::vec3(0.0f)), model_(glm::mat4(1.0f)), view_(glm::mat4(1.0f)), running_(true)
{
	glViewport(0, 0, 2560, 1440);
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);
}

void Game::run()
{
	float vertices_cube[] = 
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint vbo_cube;
	glCreateBuffers(1, &vbo_cube);
	glNamedBufferStorage(vbo_cube, sizeof(vertices_cube), vertices_cube, GL_DYNAMIC_STORAGE_BIT);

	GLuint vao_cube;
	glCreateVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);
	glVertexArrayVertexBuffer(vao_cube, 0, vbo_cube, 0, 5 * sizeof(float));
	glEnableVertexArrayAttrib(vao_cube, 0);
	glVertexArrayAttribFormat(vao_cube, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_cube, 0, 0);
	glEnableVertexArrayAttrib(vao_cube, 2);
	glVertexArrayAttribFormat(vao_cube, 2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribBinding(vao_cube, 2, 0);


	GLuint texture_wall;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_wall);
	glBindTextureUnit(0, texture_wall);
	glTextureParameteri(texture_wall, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture_wall, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture_wall, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(texture_wall, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	int width, height, channels;
	unsigned char* pixels;
	if((pixels = stbi_load("resources/images/wall.jpg", &width, &height, &channels, 0)) == nullptr)
	{
		std::cerr << "Error (stbi_load)\n";
		exit(EXIT_FAILURE);
	}
	glGenerateTextureMipmap(texture_wall);
	glTextureStorage2D(texture_wall, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(texture_wall, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);

	GLuint texture_yuri;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_yuri);
	glBindTextureUnit(1, texture_yuri);
	glTextureParameteri(texture_yuri, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture_yuri, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture_yuri, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(texture_yuri, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if((pixels = stbi_load("resources/images/yuri_tea.png", &width, &height, &channels, 0)) == nullptr)
	{
		std::cerr << "Error (stbi_load)\n";
		exit(EXIT_FAILURE);
	}
	glGenerateTextureMipmap(texture_yuri);
	glTextureStorage2D(texture_yuri, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(texture_yuri, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);

	model_ = glm::rotate(model_, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	camera_position_ = glm::vec3(0.0f, 0.0f, 10.0f);
	view_ = look_at(camera_position_, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	shader_program_.create_shader(GL_VERTEX_SHADER, "resources/shaders/base_shader.vert");
	shader_program_.create_shader(GL_FRAGMENT_SHADER, "resources/shaders/base_shader.frag");
	shader_program_.link();
	shader_program_.use();
	shader_program_.set_uniform_1i("texture_sampler0_", 0);
	shader_program_.set_uniform_1i("texture_sampler1_", 1);
	shader_program_.set_uniform_matrix_4fv("model", glm::value_ptr(model_));
	shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));
	shader_program_.set_uniform_matrix_4fv("projection", glm::value_ptr(projection));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while(running_)
	{
		handle_events();
		update();
		draw();
	}

	glDeleteVertexArrays(1, &vao_cube);
	glDeleteBuffers(1, &vbo_cube);
	glDeleteTextures(1, &texture_wall);
	glDeleteTextures(1, &texture_yuri);
}

void Game::update_arrow_keys_state()
{
	const bool* keyboard_state = SDL_GetKeyboardState(nullptr);
	arrow_keys_state_.insert_or_assign(ArrowKeys::UP, keyboard_state[SDL_SCANCODE_UP]);
	arrow_keys_state_.insert_or_assign(ArrowKeys::DOWN, keyboard_state[SDL_SCANCODE_DOWN]);
	arrow_keys_state_.insert_or_assign(ArrowKeys::LEFT, keyboard_state[SDL_SCANCODE_LEFT]);
	arrow_keys_state_.insert_or_assign(ArrowKeys::RIGHT, keyboard_state[SDL_SCANCODE_RIGHT]);
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_EVENT_KEY_DOWN:
				switch(e.key.key)
				{
					case SDLK_ESCAPE: //TODO : gérer avec SDL_GetKeyboardState ??
						running_ = false;
						break;

					default: 
						break;
				}
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				running_ = false;
				break;

			default:
				break;
		}
	}
	update_arrow_keys_state();
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_program_.use();
	model_ = glm::rotate(model_, glm::radians(float(glm::sin(SDL_GetTicks() / 1000))), glm::vec3(1.0f, 0.0f, 0.0f));
	shader_program_.set_uniform_matrix_4fv("model", glm::value_ptr(model_));
	shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	window_.swap_buffers();
}

void Game::update()
{
	//if(arrow_keys_state_.at(ArrowKeys::UP) || arrow_keys_state_.at(ArrowKeys::DOWN) || arrow_keys_state_.at(ArrowKeys::LEFT) || arrow_keys_state_.at(ArrowKeys::RIGHT))
	{
		if(arrow_keys_state_.at(ArrowKeys::UP) || arrow_keys_state_.at(ArrowKeys::DOWN))
		{
			glm::vec3 camera_forward = glm::vec3(-view_[0][2], -view_[1][2], -view_[2][2]);
			if(arrow_keys_state_.at(ArrowKeys::UP))
			{
				camera_position_ += 0.05f * camera_forward;
			}
			if(arrow_keys_state_.at(ArrowKeys::DOWN))
			{
				camera_position_ -= 0.05f * camera_forward;
			}
			//TODO : mettre cette ligne qu'une seule fois pour les deux if ??
			view_ = look_at(camera_position_, camera_position_ + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if(arrow_keys_state_.at(ArrowKeys::LEFT) || arrow_keys_state_.at(ArrowKeys::RIGHT))
		{
			glm::vec3 camera_left = glm::vec3(-view_[0][0], -view_[1][0], -view_[2][0]);
			if(arrow_keys_state_.at(ArrowKeys::LEFT))
			{
				camera_position_ += 0.05f * camera_left;
			}
			if(arrow_keys_state_.at(ArrowKeys::RIGHT))
			{
				camera_position_ -= 0.05f * camera_left;
			}
			view_ = look_at(camera_position_, camera_position_ + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

glm::mat4 Game::look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const
{
	glm::vec3 forward = glm::normalize(camera_position - camera_target_position);
	glm::vec3 left = glm::normalize(glm::cross(up_vector, forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, left));

	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -camera_position.x;
	translation[3][1] = -camera_position.y;
	translation[3][2] = -camera_position.z;

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = left.x;
	rotation[0][1] = left.y;
	rotation[0][2] = left.z;
	rotation[1][0] = up.x;
	rotation[1][1] = up.y;
	rotation[1][2] = up.z;
	rotation[2][0] = forward.x;
	rotation[2][1] = forward.y;
	rotation[2][2] = forward.z;
	rotation = glm::transpose(rotation);

	glm::mat4 view = glm::mat4(1.0f);
	view = rotation * translation;

	return view;
}