#include "game.h"
#include "gl/glew.h"
#include "stb/stb_image.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game::Game()
	: sdl_(), window_(), glew_(glewInit()), shader_program_(), 
	camera_position_(glm::vec3(0.0f)), model_(glm::mat4(1.0f)), view_(glm::mat4(1.0f)), running_(true), gamepad_(), temp_model_("resources/models/cube.gltf")
{
	glViewport(0, 0, 2560, 1440);
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);

	direction_state_.insert({Direction::UP,    {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::DOWN,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::LEFT,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::RIGHT, {false, false, 0.0f, 0.0f}});
}

void Game::run()
{
	/*std::vector<Vertex> vertices_cube;
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));

	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, -0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));

	vertices_cube.push_back(Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));

	vertices_cube.push_back(Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));

	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));

	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, 0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, 0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3( 0.5f, 0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, 0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
	vertices_cube.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));

	GLuint vbo_cube;
	glCreateBuffers(1, &vbo_cube);
	glNamedBufferStorage(vbo_cube, vertices_cube.size() * sizeof(vertices_cube[0]), vertices_cube.data(), GL_DYNAMIC_STORAGE_BIT);

	GLuint vao_cube;
	glCreateVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);
	glVertexArrayVertexBuffer(vao_cube, 0, vbo_cube, 0, sizeof(Vertex));
	glEnableVertexArrayAttrib(vao_cube, 0);
	glVertexArrayAttribFormat(vao_cube, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
	glVertexArrayAttribBinding(vao_cube, 0, 0);
	glEnableVertexArrayAttrib(vao_cube, 1);
	glVertexArrayAttribFormat(vao_cube, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color_));
	glVertexArrayAttribBinding(vao_cube, 1, 0);
	glEnableVertexArrayAttrib(vao_cube, 2);
	glVertexArrayAttribFormat(vao_cube, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinates_));
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
	stbi_image_free(pixels);*/

	model_ = glm::rotate(model_, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	camera_position_ = glm::vec3(0.0f, 1.0f, 10.0f);
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

	/*glDeleteVertexArrays(1, &vao_cube);
	glDeleteBuffers(1, &vbo_cube);
	glDeleteTextures(1, &texture_wall);
	glDeleteTextures(1, &texture_yuri);*/
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				if(e.key.key == SDLK_ESCAPE)
				{
					running_ = false;
				}
				else
				{
					if((e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_UP)
					|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP))
					{
						direction_state_.insert_or_assign(Direction::UP, DirectionInfo{true, false});
						direction_state_.at(Direction::UP).y_intensity_ = 1.0f;
					}
					if((e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_DOWN)
					|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN))
					{
						direction_state_.insert_or_assign(Direction::DOWN, DirectionInfo{true, false});
						direction_state_.at(Direction::DOWN).y_intensity_ = 1.0f;
					}
					if((e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_LEFT)
					|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT))
					{
						direction_state_.insert_or_assign(Direction::LEFT, DirectionInfo{true, false});
						direction_state_.at(Direction::LEFT).x_intensity_ = 1.0f;
					}
					if((e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RIGHT)
					|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
					{
						direction_state_.insert_or_assign(Direction::RIGHT, DirectionInfo{true, false});
						direction_state_.at(Direction::RIGHT).x_intensity_ = 1.0f;
					}
				}
				break;

			case SDL_EVENT_KEY_UP:
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				if((e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_UP)
				|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_UP && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP))
				{
					direction_state_.insert_or_assign(Direction::UP, DirectionInfo{false, false});
					direction_state_.at(Direction::UP).y_intensity_ = 0.0f;
				}
				if((e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_DOWN)
				|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_UP && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN))
				{
					direction_state_.insert_or_assign(Direction::DOWN, DirectionInfo{false, false});
					direction_state_.at(Direction::DOWN).y_intensity_ = 0.0f;
				}
				if((e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_LEFT)
				|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_UP && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT))
				{
					direction_state_.insert_or_assign(Direction::LEFT, DirectionInfo{false, false});
					direction_state_.at(Direction::LEFT).x_intensity_ = 0.0f;
				}
				if((e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_RIGHT)
				|| (e.type == SDL_EVENT_GAMEPAD_BUTTON_UP && e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
				{
					direction_state_.insert_or_assign(Direction::RIGHT, DirectionInfo{false, false});
					direction_state_.at(Direction::RIGHT).x_intensity_ = 0.0f;
				}
				break;

			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
				{
					if(e.gaxis.value > sdl::Gamepad::joystick_deadzone_) //to the right
					{
						direction_state_.insert_or_assign(Direction::RIGHT, DirectionInfo{true, true});
						direction_state_.at(Direction::RIGHT).x_intensity_ = abs(float(e.gaxis.value) / SDL_JOYSTICK_AXIS_MAX);

						//sécurité si on bascule très rapidement le joystick de droite à gauche (ne pas avoir droite et gauche à true en même temps)
						if(direction_state_.at(Direction::LEFT).is_direction_ && direction_state_.at(Direction::LEFT).is_from_joystick_)
						{
							direction_state_.insert_or_assign(Direction::LEFT, DirectionInfo{false, false});
						}
					}
					else if(e.gaxis.value < -(sdl::Gamepad::joystick_deadzone_)) //to the left
					{
						direction_state_.insert_or_assign(Direction::LEFT, DirectionInfo{true, true});
						direction_state_.at(Direction::LEFT).x_intensity_ = abs(float(e.gaxis.value) / SDL_JOYSTICK_AXIS_MAX);

						if(direction_state_.at(Direction::RIGHT).is_direction_ && direction_state_.at(Direction::RIGHT).is_from_joystick_)
						{
							direction_state_.insert_or_assign(Direction::RIGHT, DirectionInfo{false, false});
						}
					}
				}
				if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
				{
					if(e.gaxis.value > sdl::Gamepad::joystick_deadzone_) //to down
					{
						direction_state_.insert_or_assign(Direction::DOWN, DirectionInfo{true, true});
						direction_state_.at(Direction::DOWN).y_intensity_ = abs(float(e.gaxis.value) / SDL_JOYSTICK_AXIS_MAX);

						if(direction_state_.at(Direction::UP).is_direction_ && direction_state_.at(Direction::UP).is_from_joystick_)
						{
							direction_state_.insert_or_assign(Direction::UP, DirectionInfo{false, false});
						}
					}
					else if(e.gaxis.value < -(sdl::Gamepad::joystick_deadzone_)) //to up
					{
						direction_state_.insert_or_assign(Direction::UP, DirectionInfo{true, true});
						direction_state_.at(Direction::UP).y_intensity_ = abs(float(e.gaxis.value) / SDL_JOYSTICK_AXIS_MAX);

						if(direction_state_.at(Direction::DOWN).is_direction_ && direction_state_.at(Direction::DOWN).is_from_joystick_)
						{
							direction_state_.insert_or_assign(Direction::DOWN, DirectionInfo{false, false});
						}
					}
				}
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				running_ = false;
				break;

			default:
				break;
		}
	}
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_program_.use();
	//model_ = glm::rotate(model_, glm::radians(float(glm::sin(SDL_GetTicks() / 1000))), glm::vec3(1.0f, 0.0f, 0.0f));
	shader_program_.set_uniform_matrix_4fv("model", glm::value_ptr(model_));
	shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	temp_model_.draw(shader_program_);

	window_.swap_buffers();
}

void Game::update()
{
	if(direction_state_.at(Direction::UP).is_direction_ || direction_state_.at(Direction::DOWN).is_direction_)
	{
		if((direction_state_.at(Direction::UP).is_from_joystick_ || direction_state_.at(Direction::DOWN).is_from_joystick_) //Stick relâché (haut, bas)
		&& abs(gamepad_.get_axis(SDL_GAMEPAD_AXIS_LEFTY)) < gamepad_.joystick_deadzone_)
		{
			if(direction_state_.at(Direction::UP).is_direction_)
			{
				direction_state_.insert_or_assign(Direction::UP, DirectionInfo{false, false}); 
				direction_state_.at(Direction::UP).y_intensity_ = 0.0f;
			}
			if(direction_state_.at(Direction::DOWN).is_direction_)
			{
				direction_state_.insert_or_assign(Direction::DOWN, DirectionInfo{false, false});
				direction_state_.at(Direction::DOWN).y_intensity_ = 0.0f;
			}
		}
		else
		{
			glm::vec3 camera_forward = glm::vec3(-view_[0][2], -view_[1][2], -view_[2][2]);
			if(direction_state_.at(Direction::UP).is_direction_)
			{
				camera_position_ += (direction_state_.at(Direction::UP).y_intensity_ * 0.05f) * camera_forward;
			}
			if(direction_state_.at(Direction::DOWN).is_direction_)
			{
				camera_position_ -= (direction_state_.at(Direction::DOWN).y_intensity_ * 0.05f) * camera_forward;
			}
			view_ = look_at(camera_position_, camera_position_ + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	if(direction_state_.at(Direction::LEFT).is_direction_ || direction_state_.at(Direction::RIGHT).is_direction_)
	{
		if((direction_state_.at(Direction::LEFT).is_from_joystick_ || direction_state_.at(Direction::RIGHT).is_from_joystick_) //Stick relâché (gauche, droite)
		&& abs(gamepad_.get_axis(SDL_GAMEPAD_AXIS_LEFTX)) < gamepad_.joystick_deadzone_)
		{
			if(direction_state_.at(Direction::LEFT).is_direction_)
			{
				direction_state_.insert_or_assign(Direction::LEFT, DirectionInfo{false, false});
				direction_state_.at(Direction::LEFT).x_intensity_ = 0.0f;
			}
			if(direction_state_.at(Direction::RIGHT).is_direction_)
			{
				direction_state_.insert_or_assign(Direction::RIGHT, DirectionInfo{false, false});
				direction_state_.at(Direction::RIGHT).x_intensity_ = 0.0f;
			}
		}
		else
		{
			glm::vec3 camera_left = glm::vec3(-view_[0][0], -view_[1][0], -view_[2][0]);
			if(direction_state_.at(Direction::LEFT).is_direction_)
			{
				camera_position_ += (direction_state_.at(Direction::LEFT).x_intensity_ * 0.05f) * camera_left;
			}
			if(direction_state_.at(Direction::RIGHT).is_direction_)
			{
				camera_position_ -= (direction_state_.at(Direction::RIGHT).x_intensity_ * 0.05f) * camera_left;
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