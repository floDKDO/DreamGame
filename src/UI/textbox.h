#pragma once

#include "UI/text.h"
#include "Render/image.h"
#include "Input/input_manager.h"

class Textbox
{
	public:
		struct Dialogue
		{
			std::string speaker_name_;
			std::string dialogue_text_;
		};

		struct CurrentDialogueInfo
		{
			std::size_t dialogue_index_ = 0ULL; //indice dans le vector dialogues_
			std::size_t char_index_ = 0ULL; //indice sans la string du texte du dialogue courant
			bool dialogue_finished_ = false;
		};

		Textbox(const input::InputManager& input_manager);

		void set_dialogues(const std::vector<Dialogue>& dialogues);
		void render();
		void update([[maybe_unused]] float delta_time);

	private:
		void show_next_dialogue();
		void set_members_to_start_condition();
		void set_members_to_end_condition();

		static const int textbox_x_delta_;
		static const int textbox_y_delta_;
		static const int end_dialogue_indicator_x_delta_;
		static const int end_dialogue_indicator_y_delta_;
		static const int text_x_delta_;
		static const int text_y_delta_;
		static const Uint64 time_between_char_;

		const input::InputManager& input_manager_;
		Image textbox_;
		Image end_dialogue_indicator_;
		Text text_;
		std::vector<Dialogue> dialogues_;
		bool all_dialogues_shown_;
		CurrentDialogueInfo current_dialogue_info_;
		Uint64 last_time_;
};