#include "main_menu_state.h"
#include "state_machine.h"

MainMenuState::MainMenuState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend)
{}

void MainMenuState::on_show()
{
	//TODO : fondu
	std::cout << "ON SHOW! (main menu)\n";
}

void MainMenuState::on_hide()
{
	//TODO : fondu
	std::cout << "ON HIDE! (main menu)\n";
}

void MainMenuState::display()
{

}

void MainMenuState::update([[maybe_unused]] float delta_time)
{

}