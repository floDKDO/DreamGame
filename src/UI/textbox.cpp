#include "textbox.h"
#include "Resource/resource_manager.h"
#include "Common/utils.h"

const int Textbox::textbox_x_delta_ = 30;
const int Textbox::textbox_y_delta_ = 20;
const int Textbox::end_dialogue_indicator_x_delta_ = -30;
const int Textbox::end_dialogue_indicator_y_delta_ = 115;
const int Textbox::text_x_delta_ = 30;
const int Textbox::text_y_delta_ = 20;
const Uint64 Textbox::time_between_char_ = 25ULL; //valeur en ms

Textbox::Textbox(const input::InputManager& input_manager)
	: input_manager_(input_manager), textbox_("resources/images/yuri_textbox.png"), end_dialogue_indicator_("resources/images/textbox_end_dialogue_indicator.png"),
	text_("", int(textbox_.get_width()) - (text_x_delta_ * 2)), all_dialogues_shown_(false), current_dialogue_info_{}, last_time_(0ULL)
{
	textbox_.set_position(
		glm::vec2(
			(float(utils::get_viewport_width()) - textbox_.get_width()) / 2.0f,
			float(utils::get_viewport_height()) - textbox_.get_height() - float(textbox_y_delta_)
		)
	);

	end_dialogue_indicator_.set_position(
		glm::vec2(
			textbox_.get_position().x + textbox_.get_width() + float(end_dialogue_indicator_x_delta_),
			textbox_.get_position().y + float(end_dialogue_indicator_y_delta_)
		)
	);

	text_.set_position({textbox_.get_position().x + text_x_delta_, textbox_.get_position().y + text_y_delta_});
}

void Textbox::set_dialogues(const std::vector<Dialogue>& dialogues)
{
	text_.edit_text(dialogues[0].speaker_name_ + ": ");
	set_members_to_start_condition();

	for(const Dialogue& dialogue : dialogues)
	{
		dialogues_.push_back(dialogue);
	}
}

void Textbox::show_next_dialogue()
{
	current_dialogue_info_.char_index_ = 0ULL;
	current_dialogue_info_.dialogue_finished_ = false;
	text_.edit_text(dialogues_[current_dialogue_info_.dialogue_index_].speaker_name_ + ": ");
}

void Textbox::set_members_to_start_condition()
{
	dialogues_.clear();
	current_dialogue_info_ = CurrentDialogueInfo{};
	all_dialogues_shown_ = false;
	last_time_ = 0ULL;
}

void Textbox::set_members_to_end_condition()
{
	current_dialogue_info_.dialogue_index_ = 0ULL;
	current_dialogue_info_.char_index_ = 0ULL;
	current_dialogue_info_.dialogue_finished_ = true;
	all_dialogues_shown_ = true;
	last_time_ = 0ULL;
}

void Textbox::render()
{
	resource::bind_shader("Text");
	if(!all_dialogues_shown_)
	{
		text_.render();
	}

	resource::bind_shader("2d");
	if(!all_dialogues_shown_)
	{
		if(current_dialogue_info_.dialogue_finished_)
		{
			end_dialogue_indicator_.render();
		}
		textbox_.render();
	}
}

void Textbox::update([[maybe_unused]] float delta_time)
{
	Uint64 now = SDL_GetTicks();
	if(now > last_time_ + time_between_char_)
	{
		if(const Dialogue& current_dialogue = dialogues_[current_dialogue_info_.dialogue_index_]; current_dialogue_info_.char_index_ < current_dialogue.dialogue_text_.size())
		{
			text_.add_char(current_dialogue.dialogue_text_[current_dialogue_info_.char_index_]);
			current_dialogue_info_.char_index_ += 1;
		}
		else
		{
			current_dialogue_info_.dialogue_finished_ = true;
			if(input_manager_.is_interacting())
			{
				current_dialogue_info_.dialogue_index_ += 1;
				if(current_dialogue_info_.dialogue_index_ < dialogues_.size())
				{
					show_next_dialogue();
				}
				else //condition d'arrêt
				{
					set_members_to_end_condition();
				}
			}
		}
		last_time_ = now;
	}
}