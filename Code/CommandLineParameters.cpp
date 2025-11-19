// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "CommandLineParameters.hpp"

#include <iostream>
#include <optional>
#include <utility>

#include <max/Containers/StateMachine.hpp>

namespace FileReadSpeedTest {

	Action ProcessCommandLineParameters(int argc, const char * argv[]) noexcept {
		// Create state machine for parsing command line parameters
		constexpr auto start_node = max::Containers::StateMachine::MakeNode(
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [](const std::string_view& input) {
					return size_t{1};
				}
			}
		);

		auto error_message = std::optional<std::string>{std::nullopt};
		auto awaiting_flags_node = max::Containers::StateMachine::MakeNode(
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::StringMatcher{std::string_view{"-i"}}, [](const std::string_view& input) {
					return size_t{2};
				}
			},
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&error_message](const std::string_view& input) {
				error_message = "Unexpected parameter: ";
				*error_message += input;
					return size_t{3};
				}
			}
		);

		auto input_file = std::optional<std::string_view>{std::nullopt};
		auto awaiting_input_file_node = max::Containers::StateMachine::MakeNode(
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&input_file](const std::string_view& input) {
					input_file = input;
					return size_t{1};
				}
			}
		);

		constexpr auto error_node = max::Containers::StateMachine::MakeNode(
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [](const std::string_view& input) {
					return size_t{3};
				}
			}
		);

		auto state_machine = max::Containers::StateMachine::StateMachine{std::make_tuple(start_node, awaiting_flags_node, std::move(awaiting_input_file_node), std::move(error_node))};



		// Run the command line parameters through the state machine
		for (int i = 0; i < argc; i++) {
			state_machine.AttemptTransition(std::string_view{argv[i]});
		}



		// Check error conditions
		if (state_machine.current_node_index_ == 3) {
			return Action{ErrorAction{std::move(*error_message)}};
		}
		if (!input_file) {
			return Action{ErrorAction{"Error: No file provided.\nUse the form: FileReadSpeedTest.exe -i C:\\my\\file.txt"}};
		}



		return Action{SuccessAction{std::move(*input_file)}};
	}

} // namespace FileReadSpeedTest