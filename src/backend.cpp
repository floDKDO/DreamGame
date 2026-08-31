#include "backend.h"
#include "utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include <AL/al.h>
#include <stb/stb_image.h>
#include <iostream>

Backend::Backend()
	: sdl_(), window_(), glew_(glewInit())
{
	int w, h;
	window_.get_size(&w, &h);
	glViewport(0, 0, w, h);

	window_.set_relative_mouse_mode(true);

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE); //désactiver les messages de sévérité "Notification"

	//init_imgui(); //TODO
	init_openal();

	print_opengl_stuff();
}

void Backend::init_imgui() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.MouseDrawCursor = true; //afficher un curseur de souris même s'il est caché par SDL

	ImGui_ImplSDL3_InitForOpenGL(window_.fetch(), window_.get_context());
	ImGui_ImplOpenGL3_Init();
}

void Backend::init_openal()
{
	if((device_ = alcOpenDevice(nullptr)) == nullptr) //TODO : free avec alcCloseDevice
	{
		std::cout << "Error: (alcOpenDevice)\n";
	}
	if((context_ = alcCreateContext(device_, nullptr)) == nullptr) //TODO : free avec alcDestroyContext
	{
		std::cout << "Error: (alcCreateContext)\n";
	}
	if(!alcMakeContextCurrent(context_)) //TODO : free avec alcMakeContextCurrent(nullptr)
	{
		std::cout << "Error: (alcMakeContextCurrent)\n";
	}
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED); //commun pour toutes les sources
}

void Backend::get_window_size(int* w, int* h) const
{
	window_.get_size(w, h);
}

void Backend::swap_window_buffers() const
{
	window_.swap_buffers();
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

	std::cout << "[OpenGL " << lamda_type() << "] - " << lambda_severity() << lambda_source() << "ID: " << id << ", MESSAGE (length=" << length << "): \"" << message << "\"\n";
}

void print_opengl_stuff()
{
	std::cout << "OpenGL Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << std::endl;

	int max_combined_texture_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_units);

	int max_vertex_texture_units;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_vertex_texture_units);

	int max_tessellation_control_texture_units;
	glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS, &max_tessellation_control_texture_units);

	int max_tessellation_evaluation_texture_units;
	glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS, &max_tessellation_evaluation_texture_units);

	int max_geometry_texture_units;
	glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &max_geometry_texture_units);

	int max_fragment_texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_fragment_texture_units);

	int max_compute_texture_units;
	glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &max_compute_texture_units);

	std::cout << "Max combined texture units : " << max_combined_texture_units << ", including : "
		<< "\n  - max vertex shader texture units : " << max_vertex_texture_units
		<< "\n  - max tessellation control shader texture units : " << max_tessellation_control_texture_units
		<< "\n  - max tessellation evaluation shader texture units : " << max_tessellation_evaluation_texture_units
		<< "\n  - max geometry shader texture units : " << max_geometry_texture_units
		<< "\n  - max fragment shader texture units : " << max_fragment_texture_units
		<< "\n  - max compute shader texture units : " << max_compute_texture_units << "\n"
		<< std::endl;
}