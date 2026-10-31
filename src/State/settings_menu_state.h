#pragma once

#include "state.h"
#include "Input/input_manager.h"
#include "Backend/backend.h"

class SettingsMenuState : public State
{
	public:
		SettingsMenuState(const input::InputManager& input_manager, Backend& backend);
		void on_show() override;
		void on_hide() override;
		void display() override;
		void update([[maybe_unused]] float delta_time) override;

	private:
};