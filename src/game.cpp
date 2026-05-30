#include "game.h"
#include "gl/glew.h"
#include "stb/stb_image.h"
#include "tinygltf/tiny_gltf_v3.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : voir https://wikis.khronos.org/opengl/Common_Mistakes


Game::Game()
	: sdl_(), window_(), glew_(glewInit()), shader_program_(), 
	camera_position_(glm::vec3(0.0f)), model_(glm::mat4(1.0f)), view_(glm::mat4(1.0f)), running_(true), gamepad_()
{
	glViewport(0, 0, 2560, 1440);
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);

	direction_state_.insert({Direction::UP,    {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::DOWN,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::LEFT,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::RIGHT, {false, false, 0.0f, 0.0f}});

	get_info_on_gltf_file("resources/3d_models/triangle.gltf");
}

void Game::get_info_on_gltf_file(std::string_view path)
{
	std::string path_str = std::string(path);

	tg3_parse_options opts;
	tg3_error_stack errors;
	tg3_model model;

	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors);

	tg3_error_code err = tg3_parse_file(&model, &errors, path_str.c_str(), 30, &opts);
	if(err != TG3_OK)
	{
		for(uint32_t i = 0; i < errors.count; i++)
		{
			fprintf(stderr, "[%d] %s\n", (int)errors.entries[i].severity,
					errors.entries[i].message ? errors.entries[i].message : "(null)");
		}
	}

	for(size_t i = 0; i < model.nodes_count; ++i)
	{
		for(size_t j = 0; j < model.meshes_count; ++j)
		{
			for(size_t k = 0; k < model.meshes[j].primitives_count; ++k)
			{
				int32_t accessor_index_indices = model.meshes[j].primitives[k].indices;
				int32_t buffer_view_index_indices = model.accessors[accessor_index_indices].buffer_view;
				uint64_t byte_length_indices = model.buffer_views[buffer_view_index_indices].byte_length;
				uint64_t byte_offset_indices = model.buffer_views[buffer_view_index_indices].byte_offset;
				int32_t buffer_index_indices = model.buffer_views[buffer_view_index_indices].buffer;
				int32_t target_indices = model.buffer_views[buffer_view_index_indices].target;

				//std::cout << model.accessors[accessor_index_indices].component_type << std::endl; //5123 => GL_UNSIGNED_SHORT => GLushort
				std::cout << "Target: " << target_indices << std::endl; //34963 => GL_ELEMENT_ARRAY_BUFFER
				
				std::vector<GLushort> ebo_value;
				ebo_value.reserve(model.accessors[accessor_index_indices].count);

				//affiche tous les indices => devront être stockés dans un EBO 
				for(uint64_t k1 = byte_offset_indices; k1 < byte_offset_indices + byte_length_indices; k1 += sizeof(GLushort))
				{
					GLushort indice_value = (model.buffers[buffer_index_indices].data.data[k1 + 1] << 8) | model.buffers[buffer_index_indices].data.data[k1]; //=> little-endian
					std::cout << +indice_value << std::endl; //"+" pour que l'élément ne soit pas considéré comme un char mais comme un int
					ebo_value.push_back(indice_value);
				}
				/*GLuint ebo;
				glCreateBuffers(1, &ebo);
				glNamedBufferStorage(ebo, ebo_value.size() * sizeof(ebo_value[0]), ebo_value.data(), GL_DYNAMIC_STORAGE_BIT);*/

				//////////

				int32_t attributes_count = model.meshes[j].primitives[k].attributes_count;
				for(uint32_t k1 = 0; k1 < attributes_count; k1++)
				{
					std::string accessor_type_attributes = model.meshes[j].primitives[k].attributes[k1].key.data;
					int32_t accessor_index_attributes = model.meshes[j].primitives[k].attributes[k1].value;
					int32_t buffer_view_index_attributes = model.accessors[accessor_index_attributes].buffer_view;
					uint64_t byte_length_attributes = model.buffer_views[buffer_view_index_attributes].byte_length;
					uint64_t byte_offset_attributes = model.buffer_views[buffer_view_index_attributes].byte_offset;
					int32_t buffer_index_attributes = model.buffer_views[buffer_view_index_attributes].buffer;
					int32_t target_attributes = model.buffer_views[buffer_view_index_attributes].target;
					std::cout << "Target: " << target_attributes << std::endl; //34962 => GL_ARRAY_BUFFER
					std::cout << "Attribut: " << accessor_type_attributes << std::endl;

					auto lamda_type_str = [](int32_t type) -> std::string
					{
						switch(type)
						{
							case TG3_TYPE_SCALAR: return "SCALAR";
							case TG3_TYPE_VEC2:   return "VEC2";
							case TG3_TYPE_VEC3:   return "VEC3";
							case TG3_TYPE_VEC4:   return "VEC4";
							case TG3_TYPE_MAT2:   return "MAT2";
							case TG3_TYPE_MAT3:   return "MAT3";
							case TG3_TYPE_MAT4:   return "MAT4";
							default: return "";
						}
					};
					//std::cout << lamda_type_str(model.accessors[accessor_index_attributes].type) << ", " << model.accessors[accessor_index_attributes].component_type << std::endl; //VEC3 ; 5126 => GL_FLOAT => GLfloat --> glm::vec3

					std::vector<glm::vec3> position_vector;
					glm::vec3 position_vec3;

					//afficher l'attribut position (floats qui sont des composants d'un vec3)
					for(uint64_t k2 = byte_offset_attributes; k2 < byte_offset_attributes + byte_length_attributes; k2 += sizeof(GLfloat))
					{
						static int vector_counter = 0;

						uint64_t position = (model.buffers[buffer_index_attributes].data.data[k2 + 3] << 24) | (model.buffers[buffer_index_attributes].data.data[k2 + 2] << 16) | (model.buffers[buffer_index_attributes].data.data[k2 + 1] << 8) | model.buffers[buffer_index_attributes].data.data[k2];
						GLfloat position_float;
						std::memcpy(&position_float, &position, sizeof(position_float)); //obligé de faire cela pour convertir un nombre IEEE-754 en float (voir https://stackoverflow.com/questions/56710780/how-is-1-encoded-in-c-c-as-a-float-assuming-ieee-754-single-precision-represe)
						std::cout << position_float << std::endl;

						if(vector_counter == 0) //x
						{
							position_vec3.x = position_float;
						}
						else if(vector_counter == 1) //y
						{
							position_vec3.y = position_float;
						}
						else if(vector_counter == 2) //z
						{
							position_vec3.z = position_float;
						}
						vector_counter += 1;

						if(vector_counter == 3)
						{
							position_vector.push_back(position_vec3);
							position_vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
							vector_counter = 0;
						}
					}

					/*GLuint vbo;
					glCreateBuffers(1, &vbo);
					glNamedBufferStorage(vbo, position_vector.size() * sizeof(position_vector[0]), position_vector.data(), GL_DYNAMIC_STORAGE_BIT);*/

					/*for(glm::vec3& v : position_vector)
					{
						std::cout << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;
					}*/
				}	
			}
		}
	}
	tg3_model_free(&model);
	tg3_error_stack_free(&errors);
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
	model_ = glm::rotate(model_, glm::radians(float(glm::sin(SDL_GetTicks() / 1000))), glm::vec3(1.0f, 0.0f, 0.0f));
	shader_program_.set_uniform_matrix_4fv("model", glm::value_ptr(model_));
	shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));
	glDrawArrays(GL_TRIANGLES, 0, 36);

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