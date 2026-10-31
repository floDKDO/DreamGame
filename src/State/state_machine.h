#pragma once

#include "state.h"
#include "Input/input_manager.h"
#include "Backend/backend.h"

#include <iostream> //pour les classes enfants (debug)

enum class StateId
{
	TITLE_SCREEN,
	MAIN_MENU,
	SETTINGS_MENU,
	IN_GAME
};

void create_all_states(const input::InputManager& input_manager, Backend& backend);
void push_state(StateId state_id);
void pop_state();
State* get_current_state();