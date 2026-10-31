#include "settings_menu_state.h"
#include "state_machine.h"

SettingsMenuState::SettingsMenuState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend)
{}

void SettingsMenuState::on_show()
{
	//TODO : fondu
	std::cout << "ON SHOW! (settings menu)\n";
}

void SettingsMenuState::on_hide()
{
	//TODO : fondu
	std::cout << "ON HIDE! (settings menu)\n";
}

void SettingsMenuState::display()
{

}

void SettingsMenuState::update([[maybe_unused]] float delta_time)
{

}