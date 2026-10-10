#pragma once

#include "UI/text.h"
#include "Render/image.h"
#include "Input/input_manager.h"

class Textbox
{
	public:
		Textbox(const input::InputManager& input_manager);

		void set_dialogues(std::vector<std::pair<std::string, std::string>> dialogues);
		void render();

	private:
		static const int textbox_x_delta_;
		static const int textbox_y_delta_;
		static const int end_dialogue_indicator_x_delta_;
		static const int end_dialogue_indicator_y_delta_;
		static const int text_x_delta_;
		static const int text_y_delta_;

		const input::InputManager& input_manager_;
		Image textbox_;
		Image end_dialogue_indicator_;
		Text text_;
		std::vector<std::pair<std::string, std::string>> dialogues_; //speaker + dialogue text
		bool end_dialogue_;
		bool hide_textbox_;
};