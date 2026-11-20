#pragma once

#include <string_view>

namespace constants
{

inline constexpr std::string_view images_path_ = "resources/images/";
inline constexpr std::string_view shaders_path_ = "resources/shaders/";
inline constexpr std::string_view models_path_ = "resources/models/";
inline constexpr std::string_view sounds_path_ = "resources/audio/sounds/";
inline constexpr std::string_view musics_path_ = "resources/audio/musics/";
inline constexpr std::string_view fonts_path_ = "resources/fonts/";
inline constexpr std::string_view maps_path_ = "resources/maps/";
inline constexpr std::string_view cubemaps_path_ = "resources/images/cubemaps/";
inline constexpr std::string_view window_icon_path_ = "resources/images/icon.png";

inline constexpr int initial_window_width_ = 1280;
inline constexpr int initial_window_height_ = 720;

inline constexpr std::size_t undefined_resource_id_ = std::numeric_limits<std::size_t>::max();

}