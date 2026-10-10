#include "textbox.h"
#include "Resource/shader_program.h"
#include "Resource/resource_manager.h"
#include "Projection/projection.h"
#include "Common/utils.h"

#include <glm/gtc/type_ptr.hpp>

const int Textbox::textbox_x_delta_ = 30;
const int Textbox::textbox_y_delta_ = 20;
const int Textbox::end_dialogue_indicator_x_delta_ = -30;
const int Textbox::end_dialogue_indicator_y_delta_ = 115;
const int Textbox::text_x_delta_ = 30;
const int Textbox::text_y_delta_ = 20;

Textbox::Textbox(const input::InputManager& input_manager)
	: input_manager_(input_manager), textbox_("resources/images/yuri_textbox.png"), end_dialogue_indicator_("resources/images/textbox_end_dialogue_indicator.png"),
	text_("", int(textbox_.get_width()) - (text_x_delta_ * 2)), end_dialogue_(false), hide_textbox_(false)
{
	textbox_.set_position(
		glm::vec2(
			(float(utils::get_viewport_width()) - textbox_.get_width()) / 2,
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

void Textbox::set_dialogues(std::vector<std::pair<std::string, std::string>> dialogues)
{
	text_.edit_text(dialogues[0].first + ": ");
	for(const std::pair<std::string, std::string>& dialogue : dialogues)
	{
		dialogues_.push_back(dialogue);
	}
}

void Textbox::render()
{
	if(ShaderProgram* shader_program_text = resource::bind_shader("Text"); shader_program_text != nullptr)
	{
		shader_program_text->set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_orthographic_matrix()));
	}
	if(!hide_textbox_)
	{
		text_.render();
	}

	if(ShaderProgram* shader_program_2d = resource::bind_shader("2d"); shader_program_2d != nullptr)
	{
		shader_program_2d->set_uniform_matrix_4fv("projection_matrix_", glm::value_ptr(projection::get_orthographic_matrix()));
	}
	if(!hide_textbox_)
	{
		if(end_dialogue_)
		{
			end_dialogue_indicator_.render();
		}
		textbox_.render();
	}

	//TODO : faire au propre et éventuellement le mettre dans une méthode update() ??
	static Uint64 t = 0;
	static std::size_t i = 0;
	static std::size_t dialogue_index = 0;
	Uint64 now = SDL_GetTicks();
	if(now > t + 25)
	{
		if(dialogue_index >= dialogues_.size())
		{
			end_dialogue_ = true;
			hide_textbox_ = true;
			dialogue_index = 0;
			i = 0;
		}
		else
		{
			if(i < dialogues_[dialogue_index].second.size())
			{
				text_.add_char(dialogues_[dialogue_index].second[i]);
				i += 1;
			}
			else
			{
				end_dialogue_ = true;
				if(input_manager_.is_interacting())
				{
					dialogue_index += 1;
					i = 0;
					if(dialogue_index < dialogues_.size())
					{
						end_dialogue_ = false;
						//text_.clear();
						text_.edit_text(dialogues_[dialogue_index].first + ": ");
					}
				}
			}
		}
		t = now;
	}
}