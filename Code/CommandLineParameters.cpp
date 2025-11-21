// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "CommandLineParameters.hpp"

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <utility>

#include <max/Containers/StateMachine.hpp>

namespace FileReadSpeedTest {

	enum class NodeIndices : size_t {
		StartNode = 0,
		AwaitingFlagsNode = 1,
		AwaitingInputFileNode = 2,
		ErrorNode = 3,
		AwaitingBufferSizeNode = 4,
		AwaitingThreadCountNode = 5,
	};

	Action ProcessCommandLineParameters(int argc, const char * argv[]) noexcept {
		// Create state machine for parsing command line parameters
		constexpr auto start_node = max::Containers::StateMachine::MakeNode(NodeIndices::StartNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [](const std::string_view& input) {
					return NodeIndices::AwaitingFlagsNode;
				}
			}
		);

		auto error_message = std::optional<std::string>{std::nullopt};
		auto awaiting_flags_node = max::Containers::StateMachine::MakeNode(NodeIndices::AwaitingFlagsNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::StringMatcher{std::string_view{"-i"}}, [](const std::string_view& input) {
					return NodeIndices::AwaitingInputFileNode;
				}
			},
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::StringMatcher{std::string_view{"--buffer_size"}}, [](const std::string_view& input) {
					return NodeIndices::AwaitingBufferSizeNode;
				}
			},
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::StringMatcher{std::string_view{"--thread_count"}}, [](const std::string_view& input) {
					return NodeIndices::AwaitingThreadCountNode;
				}
			},
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&error_message](const std::string_view& input) {
					error_message = "Unexpected parameter: ";
					*error_message += input;
					return NodeIndices::ErrorNode;
				}
			}
		);

		auto input_file = std::optional<std::string_view>{std::nullopt};
		auto awaiting_input_file_node = max::Containers::StateMachine::MakeNode(NodeIndices::AwaitingInputFileNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&input_file](const std::string_view& input) {
					input_file = input;
					return NodeIndices::AwaitingFlagsNode;
				}
			}
		);

		constexpr auto error_node = max::Containers::StateMachine::MakeNode(NodeIndices::ErrorNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [](const std::string_view& input) {
					return NodeIndices::ErrorNode;
				}
			}
		);

		auto buffer_size = std::optional<size_t>{std::nullopt};
		auto awaiting_buffer_size_node = max::Containers::StateMachine::MakeNode(NodeIndices::AwaitingBufferSizeNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&buffer_size, &error_message](const std::string_view& input) {
					// TODO: Maybe use std::stoull here instead of std::strtoull. But that takes a string, not a string_view.
					char * end = nullptr;
					unsigned long long temp = std::strtoull(input.data(), &end, /*base=*/10);
					if (errno == ERANGE) {
						error_message = "Could not convert parameter to number: ";
						*error_message += input;
						return NodeIndices::ErrorNode;
					}
					buffer_size = temp;
					return NodeIndices::AwaitingFlagsNode;
				}
			}
		);

		auto thread_count = std::optional<size_t>{std::nullopt};
		auto awaiting_thread_count_node = max::Containers::StateMachine::MakeNode(NodeIndices::AwaitingThreadCountNode,
			max::Containers::StateMachine::Transition{
				max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, [&thread_count, &error_message](const std::string_view& input) {
					// TODO: Also here
					char * end = nullptr;
					unsigned long long temp = std::strtoull(input.data(), &end, /*base=*/10);
					if (errno == ERANGE) {
						error_message = "Could not convert parameter to number: ";
						*error_message += input;
						return NodeIndices::ErrorNode;
					}
					thread_count = temp;
					return NodeIndices::AwaitingFlagsNode;
				}
			}
		);

		auto state_machine = max::Containers::StateMachine::StateMachine{NodeIndices::StartNode, std::make_tuple(start_node, awaiting_flags_node, std::move(awaiting_input_file_node), std::move(error_node), awaiting_buffer_size_node, awaiting_thread_count_node)};



		// Run the command line parameters through the state machine
		for (int i = 0; i < argc; i++) {
			state_machine.AttemptTransition(std::string_view{argv[i]});
		}



		// Check error conditions
		if (state_machine.current_node_index_ == NodeIndices::ErrorNode) {
			return Action{ErrorAction{std::move(*error_message)}};
		}
		if (!input_file) {
			return Action{ErrorAction{"Error: No file provided.\nUse the form: FileReadSpeedTest.exe [options] -i C:\\my\\file.txt"}};
		}



		return Action{SuccessAction{std::move(*input_file), std::move(buffer_size), std::move(thread_count)}};
	}

} // namespace FileReadSpeedTest