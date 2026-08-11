#include "utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace utils
{

std::string get_string_from_file(std::string_view path)
{
	std::string path_str = std::string(path);
	std::ifstream file(path_str);
	if(!file.is_open())
	{
		std::cerr << "Error: unable to open the requested file (" << path << ")!\n";
	}

	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
}

glm::vec3 get_camera_forward(glm::mat4 view_matrix)
{
	return glm::vec3(-view_matrix[0][2], -view_matrix[1][2], -view_matrix[2][2]);
}

glm::vec3 get_camera_left(glm::mat4 view_matrix)
{
	return glm::vec3(-view_matrix[0][0], -view_matrix[1][0], -view_matrix[2][0]);
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

GLfloat ieee754_to_float(uint64_t ieee754_number)
{
	GLfloat float_number;
	std::memcpy(&float_number, &ieee754_number, sizeof(float_number)); //obligé de faire cela pour convertir un nombre IEEE-754 en float (voir https://stackoverflow.com/questions/56710780/how-is-1-encoded-in-c-c-as-a-float-assuming-ieee-754-single-precision-represe)
	return float_number;
}

}