#include "backend.h"
#include "Common/utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"
#include "gl_resource_manager.h"
#include "Logging/logging.h"

#include <AL/al.h>
#include <stb/stb_image.h>
#include <iostream>

Backend::Backend()
	: sdl_(), window_(), glew_(glewInit())
{
	logging::create(logging::Severity::CRITICAL);

	resource::add_shader("Phong", {"resources/shaders/phong_shader.vert", "resources/shaders/phong_shader.frag"});
	resource::bind_shader("Phong");

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

Backend::~Backend()
{
	destroy_openal();
	//destroy_imgui();
}

void Backend::handle_events(const SDL_Event& e)
{
	if(e.type == SDL_EVENT_WINDOW_RESIZED)
	{
		glViewport(0, 0, e.window.data1, e.window.data2);
	}
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

void Backend::destroy_imgui() const
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void Backend::init_openal()
{
	if((device_ = alcOpenDevice(nullptr)) == nullptr)
	{
		logging::log("alcOpenDevice() returned nullptr!", logging::Severity::CRITICAL);
	}
	if((context_ = alcCreateContext(device_, nullptr)) == nullptr)
	{
		logging::log("alcCreateContext() returned nullptr!", logging::Severity::CRITICAL);
	}
	if(!alcMakeContextCurrent(context_))
	{
		logging::log("alcMakeContextCurrent() returned an error!", logging::Severity::CRITICAL);
	}
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED); //commun pour toutes les sources
}

void Backend::destroy_openal()
{
	if(!alcMakeContextCurrent(nullptr))
	{
		logging::log("alcMakeContextCurrent() returned an error!", logging::Severity::CRITICAL);
	}
	alcDestroyContext(context_);
	if(!alcCloseDevice(device_))
	{
		logging::log("alcCloseDevice() returned an error!", logging::Severity::CRITICAL);
	}
}

glm::ivec2 Backend::get_window_size() const
{
	int w, h;
	window_.get_size(&w, &h);
	return glm::ivec2(w, h);
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

	logging::log("[OpenGL " + lamda_type() + "] - " + lambda_severity() + lambda_source() + "ID: " + std::to_string(id) + ", MESSAGE (length=" + std::to_string(length) + "): \"" + message + "\"", logging::Severity::DEBUG);
}

void print_opengl_stuff()
{
	logging::log("OpenGL Vendor: " + utils::get_string_from_unsigned_char_ptr(glGetString(GL_VENDOR)), logging::Severity::DEBUG);
	logging::log("OpenGL Renderer: " + utils::get_string_from_unsigned_char_ptr(glGetString(GL_RENDERER)), logging::Severity::DEBUG);
	logging::log("OpenGL Version: " + utils::get_string_from_unsigned_char_ptr(glGetString(GL_VERSION)), logging::Severity::DEBUG);
	logging::log("OpenGL Shading Language Version: " + utils::get_string_from_unsigned_char_ptr(glGetString(GL_SHADING_LANGUAGE_VERSION)), logging::Severity::DEBUG);
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

	logging::log("Max combined texture units: " + std::to_string(max_combined_texture_units) + ", including: "
		 + "\n  - max vertex shader texture units: " + std::to_string(max_vertex_texture_units)
		 + "\n  - max tessellation control shader texture units: " + std::to_string(max_tessellation_control_texture_units)
		 + "\n  - max tessellation evaluation shader texture units: " + std::to_string(max_tessellation_evaluation_texture_units)
		 + "\n  - max geometry shader texture units: " + std::to_string(max_geometry_texture_units)
		 + "\n  - max fragment shader texture units: " + std::to_string(max_fragment_texture_units)
		 + "\n  - max compute shader texture units: " + std::to_string(max_compute_texture_units), logging::Severity::DEBUG);
	std::cout << std::endl;
}