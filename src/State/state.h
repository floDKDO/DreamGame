#pragma once

#include "Input/input_manager.h"
#include "Backend/backend.h"

class State
{
	public:
		virtual ~State() = default;
		virtual void on_show() = 0;
		virtual void on_hide() = 0;
		virtual void display() = 0;
		virtual void update(float delta_time) = 0;
	
	protected:
		State(const input::InputManager& input_manager, Backend& backend);

		const input::InputManager& input_manager_;
		Backend& backend_;

	private:
};