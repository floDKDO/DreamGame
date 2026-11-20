#include "title_screen_state.h"
#include "state_machine.h"

TitleScreenState::TitleScreenState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend)
{}

void TitleScreenState::on_show()
{
	//TODO : fondu
	std::cout << "ON SHOW! (title screen)\n";
}

void TitleScreenState::on_hide()
{
	//TODO : fondu
	std::cout << "ON HIDE! (title screen)\n";
}

void TitleScreenState::display()
{

}

void TitleScreenState::update([[maybe_unused]] float delta_time)
{

}