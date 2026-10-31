#pragma once

#include "state.h"
#include "Resource/model.h"
#include "Map/map_file.h"
#include "Image/image.h"
#include "Render/cubemap.h"
#include "UI/textbox.h"
#include "Backend/backend.h"
#include "Camera/camera.h"
#include "Player/player.h"

class InGameState : public State
{
	public:
		InGameState(const input::InputManager& input_manager, Backend& backend);
		void on_show() override;
		void on_hide() override;
		void display() override;
		void update([[maybe_unused]] float delta_time) override;

	private:
		Player player_;
		Camera camera_;
		MapFile test_map_;
		Model* gizmo_;
		Image test_image_;
		Textbox textbox_;
		Cubemap skybox_;
};