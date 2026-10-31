#include "title_screen_state.h"
#include "state_machine.h"

TitleScreenState::TitleScreenState(const input::InputManager& input_manager, Backend& backend)
	: State(input_manager, backend), test_image_("resources/images/nakazato.jpg")
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
	test_image_.render();
}

void TitleScreenState::update([[maybe_unused]] float delta_time)
{
	if(input_manager_.is_interacting())
	{
		pop_state();
	}
}