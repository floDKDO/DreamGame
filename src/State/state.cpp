#include "state.h"

State::State(const input::InputManager& input_manager, Backend& backend)
: input_manager_(input_manager), backend_(backend)
{}