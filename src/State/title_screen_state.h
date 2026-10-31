#pragma once

#include "state.h"
#include "Image/image.h"
#include "Input/input_manager.h"
#include "Backend/backend.h"

class TitleScreenState : public State
{
	public:
		TitleScreenState(const input::InputManager& input_manager, Backend& backend);
		void on_show() override;
		void on_hide() override;
		void display() override;
		void update([[maybe_unused]] float delta_time) override;

	private:
		Image test_image_;
};