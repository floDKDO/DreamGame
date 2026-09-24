#include "game.h"
#include "OpenAL/openal.h"
#include "gl_resource_manager.h"
#include "Render/projection.h"

//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_sdl3.h"
//#include "imgui/imgui_impl_opengl3.h"

//#include <SDL3_ttf/SDL_ttf.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : créer des constantes pour les valeurs par défaut de translation, rotation et scale

Game::Game()
	: backend_(), //window_(),
	player_(input_manager_),
	camera_(input_manager_, player_.get_model()->get_position()),
	running_(true), gamepad_(), test_map_("resources/maps/corridor.gltf"), 
	gizmo_("resources/models/axis_gizmo.glb")
{}

void Game::run()
{
	glm::ivec2 window_size = backend_.get_window_size();

	std::string temp_model_name("test"); //ici, "test" serait le nom du modèle
	audio::set_listener_position(player_.get_model()->get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());
	audio::set_listener_velocity(glm::vec3(0.0f));
	audio::create_source(temp_model_name, "resources/audio/test.wav"); 
	audio::set_source_gain(temp_model_name, 1.0f);
	audio::set_source_gain(temp_model_name, 1.0f);
	audio::set_source_pitch(temp_model_name, 1.0f);
	audio::set_source_reference_distance(temp_model_name, 1.0f);
	audio::set_source_max_distance(temp_model_name, 20.0f);
	audio::set_source_rolloff_factor(temp_model_name, 1.0f);
	audio::set_source_position(temp_model_name, glm::vec3(0.0f));
	//audio::play_source(temp_model_name);

	Uint64 begin_current_frame = 0; //en ms
	Uint64 last_frame = 0;          //en ms
	float  delta_time = 0;          //en secondes
	Uint64 last_fps_refresh = 0;    //en ms
	unsigned int frame_count_this_second = 0; //nombre de frames par seconde

	while(running_)
	{
		begin_current_frame = SDL_GetTicks();
		delta_time = (float(begin_current_frame) - float(last_frame)) / 1000.0f; //conversion en secondes

		handle_events();

		//TODO
		////////////////////////////////////////////////////////////////////////////////////////
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplSDL3_NewFrame();
		//ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)
		////////////////////////////////////////////////////////////////////////////////////////

		update(delta_time);
		draw();

		update_fps_count(last_fps_refresh, frame_count_this_second);
		last_frame = begin_current_frame;
	}

	audio::destroy_source(temp_model_name);
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
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
		backend_.handle_events(e);
		input_manager_.handle_events(e);

		////////////////////////////////////////////////////////////////////////////////////////
		//ImGui_ImplSDL3_ProcessEvent(&e); //TODO
		////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//resource::set_uniform_1i("texture_sampler0_", 0);
	glm::ivec2 window_size = backend_.get_window_size();
	resource::set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_perspective_matrix(float(window_size.x) / float(window_size.y))));

	player_.draw();
	gizmo_.draw();
	test_map_.draw();

	audio::set_listener_position(player_.get_model()->get_position());
	audio::set_listener_orientation(camera_.get_camera_forward(), camera_.get_camera_up());

	//TODO
	////////////////////////////////////////////////////////////////////////////////////////
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	////////////////////////////////////////////////////////////////////////////////////////

	backend_.swap_window_buffers(); //TODO : pas ouf, devrait directement être dans la classe Backend
}

void Game::update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const
{
	frame_count_this_second += 1;
	if(SDL_GetTicks() >= last_fps_refresh + 1000) //tester une fois par seconde pour obtenir des frames par seconde
	{
		//window_.update_fps(frame_count_this_second); //TODO
		last_fps_refresh = SDL_GetTicks();
		frame_count_this_second = 0;
	}
}

std::optional<std::pair<glm::vec3, AABB>> collision_detection(const gltf::Node& map_node, const gltf::Node& player_node)
{
	if(!map_node.is_empty_node()) 
	{
		std::optional<AABB> map_node_aabb_optional = map_node.get_world_aabb();
		if(!map_node_aabb_optional.has_value())
		{
			return std::nullopt;
		}

		const AABB& map_node_aabb = map_node_aabb_optional.value();
		std::optional<AABB> player_node_aabb_optional = player_node.get_world_aabb();
		const AABB& player_node_aabb = player_node_aabb_optional.value(); //on part du principe que le joueur aura toujours un AABB

		if(player_node_aabb.intersection_with_aabb(map_node_aabb))
		{
			return std::make_pair(player_node_aabb.get_overlap_with_aabb(map_node_aabb), map_node_aabb);
		}
		else
		{
			return std::nullopt;
		}
	}

	for(const gltf::Node& child_node : map_node.get_children_nodes())
	{
		std::optional<std::pair<glm::vec3, AABB>> result = collision_detection(child_node, player_node);
		if(result.has_value())
		{
			return result;
		}
	}
	return std::nullopt;
}

void collision_response(const std::pair<glm::vec3, AABB>& collision_info, gltf::Node& player_node)
{
	const AABB& map_node_aabb = collision_info.second;
	std::optional<AABB> player_node_aabb_optional = player_node.get_world_aabb();
	const AABB& player_node_aabb = player_node_aabb_optional.value(); //on part du principe que le joueur aura toujours un AABB

	glm::vec3 player_center = player_node_aabb.get_center();
	glm::vec3 map_node_center = map_node_aabb.get_center();
	glm::vec3 overlap = collision_info.first;

	//on cherche le plus petit overlap car on veut déplacer le joueur de la plus petite distance possible pour qu'il ne soit plus en collision avec le modèle
	if(overlap.x < overlap.y && overlap.x < overlap.z)
	{
		player_node.add_translation_x((player_center.x < map_node_center.x) ? -overlap.x : overlap.x);
	}
	else if(overlap.y < overlap.z)
	{
		player_node.add_translation_y((player_center.y < map_node_center.y) ? -overlap.y : overlap.y);
	}
	else
	{
		player_node.add_translation_z((player_center.z < map_node_center.z) ? -overlap.z : overlap.z);
	}
}

void Game::update(float delta_time)
{
	camera_.update(delta_time);
	player_.update(delta_time, camera_.get_camera_forward(), camera_.get_camera_left());
	gamepad_.check(1000); //tester une fois par seconde
	input_manager_.update(delta_time);

	for(const std::unique_ptr<Model>& map_model : test_map_.get_models())
	{
		if(std::optional<std::pair<glm::vec3, AABB>> collision_info = collision_detection(map_model->get_root_node(), player_.get_model()->get_root_node()); collision_info.has_value())
		{
			collision_response(collision_info.value(), player_.get_model()->get_root_node());
		}
	}
}