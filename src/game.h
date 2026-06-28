#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "RAII_SDL3/gamepad.h"
#include "model.h"

#include <unordered_map>

class Game
{
	public:
		Game();

		void run();
		void handle_events();
		void draw();
		void update();

	private:
		enum class Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		struct DirectionInfo
		{
			bool is_direction_;
			bool is_from_joystick_;
			float x_intensity_; //joystick value
			float y_intensity_; //joystick value
		};

		struct EulerAngles
		{
			EulerAngles()
				: pitch_(0.0f), yaw_(-90.0f), roll_(0.0f)
			{}

			float pitch_;
			float yaw_;
			float roll_; //inutilisé pour l'instant
		};

		struct MouseInfo
		{
			MouseInfo()
				: x_pos_(0.0f), y_pos_(0.0f), last_x_pos_(0.0f), last_y_pos_(0.0f), x_rel_(0.0f), y_rel_(0.0f), is_mouse_moving_(false)
			{}

			void set_info(float x_pos, float y_pos, float x_rel, float y_rel)
			{
				last_x_pos_ = x_pos_;
				last_y_pos_ = y_pos_;
				x_pos_ = x_pos;
				y_pos_ = y_pos;
				x_rel_ = x_rel;
				y_rel_ = y_rel;
				is_mouse_moving_ = true;
			}

			float x_pos_, y_pos_; //TODO : inutile pour l'instant
			float last_x_pos_, last_y_pos_; //TODO : inutile pour l'instant
			float x_rel_, y_rel_;
			bool is_mouse_moving_;
		};

		struct RightJoystickInfo
		{
			float x_axis_, y_axis_;
		};

		enum class CameraRotationSource
		{
			MOUSE,
			GAMEPAD
		};

		void calculate_euler_angles(CameraRotationSource source);
		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ShaderProgram shader_program_;
		glm::vec3 camera_position_;
		glm::vec3 camera_front_;
		EulerAngles euler_angles_;
		MouseInfo mouse_info_;
		RightJoystickInfo right_joystick_info_;
		glm::mat4 model_;
		glm::mat4 view_;
		bool running_;
		sdl::Gamepad gamepad_;
		std::unordered_map<Direction, DirectionInfo> direction_state_;

		Model temp_model_;
};

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param);