#include "game.h"

#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game::Game()
	: sdl_(), window_(), glew_(glewInit()), shader_program_(), 
	camera_position_(glm::vec3(0.0f)), camera_front_(glm::vec3(0.0f, 0.0f, -1.0f)), euler_angles_(), mouse_info_(), right_joystick_info_(),
	model_(glm::mat4(1.0f)), view_(glm::mat4(1.0f)), running_(true), gamepad_(), temp_model_("resources/models/texture.gltf")
{
	int w, h;
	window_.get_size(&w, &h); 
	glViewport(0, 0, w, h);

	SDL_SetWindowRelativeMouseMode(window_.fetch(), true);

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(message_callback, nullptr);

	direction_state_.insert({Direction::UP,    {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::DOWN,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::LEFT,  {false, false, 0.0f, 0.0f}});
	direction_state_.insert({Direction::RIGHT, {false, false, 0.0f, 0.0f}});
}

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param)
{
	auto lambda_source = [&source]() -> std::string
	{
		switch(source)
		{
			case GL_DEBUG_SOURCE_API:
				return "SOURCE: API, ";

			case GL_DEBUG_SOURCE_APPLICATION:
				return "SOURCE: Application, ";

			case GL_DEBUG_SOURCE_OTHER:
				return "SOURCE: Other, ";

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "SOURCE: Shader compiler, ";

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "SOURCE: Third party, ";

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "SOURCE: Window system, ";

			default:
				return "SOURCE: Unknown, ";
		}
	};

	auto lamda_type = [&type]() -> std::string
	{
		switch(type)
		{
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				return "Deprecated behavior";

			case GL_DEBUG_TYPE_ERROR:
				return "Error";

			case GL_DEBUG_TYPE_MARKER:
				return "Marker";

			case GL_DEBUG_TYPE_OTHER:
				return "Other";

			case GL_DEBUG_TYPE_PERFORMANCE:
				return "Performance";

			case GL_DEBUG_TYPE_POP_GROUP:
				return "Pop group";

			case GL_DEBUG_TYPE_PORTABILITY:
				return "Portability";

			case GL_DEBUG_TYPE_PUSH_GROUP:
				return "Push group";

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				return "Undefined behavior";

			default:
				return "Unknown";
		}
	};

	auto lambda_severity = [&severity]() -> std::string
	{
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				return "SEVERITY: High, ";

			case GL_DEBUG_SEVERITY_LOW:
				return "SEVERITY: Low, ";

			case GL_DEBUG_SEVERITY_MEDIUM:
				return "SEVERITY: Medium, ";

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				return "SEVERITY: Notification, ";

			default:
				return "SEVERITY: Unknown, ";
		}
	};

	std::cout << "[OpenGL " << lamda_type() << "] - " << lambda_severity() << lambda_source() << "ID: " << id << ", MESSAGE (length=" << length << "): <" << message << ">\n";
}

void Game::run()
{
	//model_ = glm::rotate(model_, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	camera_position_ = glm::vec3(0.0f, 0.5f, 5.0f);
	view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));

	int w, h;
	window_.get_size(&w, &h);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 100.0f);

	shader_program_.create_shader(GL_VERTEX_SHADER, "resources/shaders/base_shader.vert");
	shader_program_.create_shader(GL_FRAGMENT_SHADER, "resources/shaders/base_shader.frag");
	shader_program_.link();
	shader_program_.use();
	shader_program_.set_uniform_1i("texture_sampler0_", 0);
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
				if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
				{
					right_joystick_info_.x_axis_ = e.gaxis.value;
				}
				if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
				{
					right_joystick_info_.y_axis_ = e.gaxis.value;
				}
				break;

			case SDL_EVENT_MOUSE_MOTION:
				mouse_info_.set_info(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
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

template<typename Arithmetic1, typename Arithmetic2, typename Arithmetic3>
double lerp(Arithmetic1 a, Arithmetic2 b, Arithmetic3 t) //TODO : à terme, remplacer par la fonction std::lerp de C++20
{
	double a_d = static_cast<double>(a);
	double b_d = static_cast<double>(b);
	double t_d = static_cast<double>(t);

	return a_d + t_d * (b_d - a_d);
}

void Game::update()
{
	//TODO : créer une fonction pour le code suivant
	static Uint64 last_time = 0, current_time = 0;
	current_time = SDL_GetTicks();
	if(!gamepad_.is_open() && current_time > last_time + 1000) //tester une fois par seconde
	{
		gamepad_.open();
		last_time = current_time;
	}

	static bool should_reset = false;
	static float saved_yaw = euler_angles_.yaw_;
	static float saved_pitch = euler_angles_.pitch_;
	static float temp = 0.0f;
	//TODO : rencentrer la vue sur 0 si aucun mouvement de la caméra ??
	if(mouse_info_.is_mouse_moving_)
	{
		calculate_euler_angles(CameraRotationSource::MOUSE);
		should_reset = true;
		saved_yaw = euler_angles_.yaw_;
		saved_pitch = euler_angles_.pitch_;
		temp = 0.0f;

	}
	else if(abs(right_joystick_info_.x_axis_) > sdl::Gamepad::joystick_deadzone_ 
	     || abs(right_joystick_info_.y_axis_) > sdl::Gamepad::joystick_deadzone_)
	{
		calculate_euler_angles(CameraRotationSource::GAMEPAD);
		should_reset = true;
		saved_yaw = euler_angles_.yaw_;
		saved_pitch = euler_angles_.pitch_;
		temp = 0.0f;
	}
	else
	{
		//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;
		if(should_reset)
		{
			
			//std::cout << lerp(euler_angles_.yaw_, -90.0f, temp) << ", " << lerp(euler_angles_.pitch_, 0.0f, temp) << std::endl;

			float yaw = lerp(saved_yaw, -90.0f, temp);
			float pitch = lerp(saved_pitch, 0.0f, temp);
			
			euler_angles_.yaw_ = yaw;
			euler_angles_.pitch_ = pitch;

			glm::vec3 front = glm::vec3(
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
			);
			camera_front_ = glm::normalize(front);
			view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
			shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));

			//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;

			temp += 0.01f;
			std::cout << "TEMP: " << temp << ", YAW: " << yaw << ", PITCH: " << pitch << std::endl;
			if(temp > 1.0f)
			{
				temp = 0.0f;
				should_reset = false;
				//right_joystick_info_.x_axis_ = 0.0f;
				//right_joystick_info_.y_axis_ = 0.0f;

				//euler_angles_.yaw_ = yaw;
				//euler_angles_.pitch_ = pitch;

				//std::cout << right_joystick_info_.x_axis_ << ", " << right_joystick_info_.y_axis_ << std::endl;
				//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;
			}
		}
	}

	//TODO : créer une fonction pour le code suivant
	if(direction_state_.at(Direction::UP).is_direction_ || direction_state_.at(Direction::DOWN).is_direction_)
	{
		if((direction_state_.at(Direction::UP).is_from_joystick_ || direction_state_.at(Direction::DOWN).is_from_joystick_) //Stick relâché (haut, bas)
		&& abs(gamepad_.get_axis(SDL_GAMEPAD_AXIS_LEFTY)) < sdl::Gamepad::joystick_deadzone_)
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
			view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	if(direction_state_.at(Direction::LEFT).is_direction_ || direction_state_.at(Direction::RIGHT).is_direction_)
	{
		if((direction_state_.at(Direction::LEFT).is_from_joystick_ || direction_state_.at(Direction::RIGHT).is_from_joystick_) //Stick relâché (gauche, droite)
		&& abs(gamepad_.get_axis(SDL_GAMEPAD_AXIS_LEFTX)) < sdl::Gamepad::joystick_deadzone_)
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
			view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	camera_position_.y = 0.5f; //TODO : doit être égal à la hauteur du sol
	shader_program_.set_uniform_3f("view_position_", camera_position_);
}

void Game::calculate_euler_angles(CameraRotationSource source)
{
	if(source == CameraRotationSource::MOUSE)
	{
		euler_angles_.yaw_ += (std::min(mouse_info_.x_rel_, 15.0f) / 15.0f); //TODO : hardcodé
		euler_angles_.pitch_ += (std::min(-mouse_info_.y_rel_, 15.0f) / 15.0f); // "-" pour que le sens soit bon (mettre la souris vers le haut déplace la caméra vers le haut). Enlever "-" pour inverser la caméra
		mouse_info_.is_mouse_moving_ = false;
	}
	else if(source == CameraRotationSource::GAMEPAD)
	{
		float sensitivity = 0.5f;
		euler_angles_.yaw_ += (right_joystick_info_.x_axis_ / SDL_JOYSTICK_AXIS_MAX) * sensitivity;
		euler_angles_.pitch_ += -(right_joystick_info_.y_axis_ / SDL_JOYSTICK_AXIS_MAX) * sensitivity; // "-" pour que le sens soit bon (mettre la souris vers le haut déplace la caméra vers le haut). Enlever "-" pour inverser la caméra
	}

	//TODO : std::max/std::min/std::clamp ?
	if(euler_angles_.pitch_ > 25.0f)
	{
		euler_angles_.pitch_ = 25.0f;
	}
	else if(euler_angles_.pitch_ < -25.0f)
	{
		euler_angles_.pitch_ = -25.0f;
	}

	//TODO : std::max/std::min/std::clamp ?
	if(euler_angles_.yaw_ > -50.0f)
	{
		euler_angles_.yaw_ = -50.0f;
	}
	else if(euler_angles_.yaw_ < -130.0f)
	{
		euler_angles_.yaw_ = -130.0f;
	}

	glm::vec3 front = glm::vec3(
		cos(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_))
	);
	camera_front_ = glm::normalize(front);
	view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
	shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));
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