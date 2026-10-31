#include "state_machine.h"
#include "in_game_state.h"
#include "title_screen_state.h"

#include <stack>
#include <unordered_map>
#include <memory>

std::stack<std::pair<StateId, State*>> state_stack_;
std::unordered_map<StateId, std::unique_ptr<State>> state_map_;

void create_all_states(const input::InputManager& input_manager, Backend& backend)
{
	state_map_.insert({StateId::TITLE_SCREEN, std::make_unique<TitleScreenState>(input_manager, backend)});
	state_map_.insert({StateId::IN_GAME , std::make_unique<InGameState>(input_manager, backend)});
	push_state(StateId::IN_GAME);
}

void push_state(StateId state_id)
{
	if(!state_stack_.empty())
	{
		state_stack_.top().second->on_hide();
	}
	state_stack_.push({state_id, state_map_.at(state_id).get()});
	state_stack_.top().second->on_show();
}

void pop_state()
{
	if(!state_stack_.empty())
	{
		state_stack_.top().second->on_hide();
	}
	state_stack_.pop();
	if(!state_stack_.empty())
	{
		state_stack_.top().second->on_show();
	}
}

State* get_current_state()
{
	return state_stack_.top().second;
}