// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstdint>
#include <iostream>
#include <optional>
#include <string_view>
#include <thread>
#include <tuple>
#include <utility>

#include "ThreadPool.hpp"
#include "OverlappedIOFileRead.hpp"
#include "OverlappedIOFileReadTask.hpp"

#include <max/Containers/StateMachine/AnythingMatcher.hpp>
#include <max/Containers/StateMachine/Node.hpp>
#include <max/Containers/StateMachine/StateMachine.hpp>
#include <max/Containers/StateMachine/StringMatcher.hpp>
#include <max/Containers/StateMachine/Transition.hpp>

int main(int argc, const char * argv[]) {
	// Create state machine for parsing command line parameters
	auto program_name_callback = [](const std::string_view& input) {
		return size_t{1};
	};
	auto program_name_transition = max::Containers::StateMachine::Transition{max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, std::move(program_name_callback)};
	auto start_node = max::Containers::StateMachine::MakeNode(std::move(program_name_transition));

	auto input_flag_callback = [](const std::string_view& input) {
		return size_t{2};
	};
	auto input_flag_transition = max::Containers::StateMachine::Transition{max::Containers::StateMachine::StringMatcher{std::string_view{"-i"}}, std::move(input_flag_callback)};
	auto awaiting_flags_node = max::Containers::StateMachine::MakeNode(std::move(input_flag_transition));

	auto input_file = std::optional<std::string_view>{std::nullopt};
	auto input_file_callback = [&input_file](const std::string_view& input) {
		input_file = input;
		return size_t{1};
	};
	auto input_file_transition = max::Containers::StateMachine::Transition{max::Containers::StateMachine::AnythingMatcher<std::string_view>{}, std::move(input_file_callback)};
	auto awaiting_input_file_node = max::Containers::StateMachine::MakeNode(std::move(input_file_transition));

	auto state_machine = max::Containers::StateMachine::StateMachine{std::make_tuple(std::move(start_node), std::move(awaiting_flags_node), std::move(awaiting_input_file_node))};

	for (int i = 0; i < argc; i++) {
		state_machine.AttemptTransition(std::string_view{argv[i]});
	}



	// Check command line parameters.
	if (!input_file) {
		std::cerr << "Error: No file provided.\n";
		std::cerr << "Use the form: FileReadSpeedTest.exe -i C:\\my\\file.txt" << std::endl;
		return -1;
	}


	// Notify user of clock stability.
	if (std::chrono::high_resolution_clock::is_steady) {
		std::cout << "Clock is steady" << std::endl;
	} else {
		std::cout << "Clock is not steady" << std::endl;
	}


	// Create thread pool.
	// Some workloads are inherently serial and may not benefit from additional threads.
	// Workloads that may benefit could require heavy or light processing.
	// All of this determines the type and quantity of cores to be used.
	auto thread_pool = FileReadSpeedTest::CreateThreadPool();
	if (!thread_pool.has_value()) {
		std::cerr << "Could not create thread pool\n";
		return -1;
	}
	size_t worker_thread_count = thread_pool->task_threads_.size();
	std::cout << "Worker thread count: " << worker_thread_count << std::endl;



	// Prepare to read the file.
	auto overlapped_io_file_read = FileReadSpeedTest::PrepareToReadFile(*input_file, worker_thread_count);
	if (!overlapped_io_file_read.has_value()) {
		std::cerr << "Error reading file\n";
		return -1;
	}


	// Create main task queue to receive completion notifications
	auto main_task_queue = max::Hardware::CPU::CreateTaskQueue();
	if (!main_task_queue.has_value()) {
		std::cerr << "Could not create main task queue\n";
		return -1;
	}
	size_t completed_threads = 0;
	

	// Issue initial reads.
	overlapped_io_file_read->Read();


	// Add initial read tasks to the thread pool
	size_t thread_count = thread_pool->task_threads_.size();
	size_t buffer_size = overlapped_io_file_read->contexts_[0].bytes_to_read_;
	LARGE_INTEGER current_read_start = {};
	size_t i = 0;
	for (auto& thread: thread_pool->task_threads_) {
		auto add_task_result = thread.task_queue_->AddTask(std::make_unique<FileReadSpeedTest::OverlappedIOFileReadTask>(&overlapped_io_file_read.value(), current_read_start, thread_count * buffer_size, i, thread_count, thread.task_queue_.get(), main_task_queue->get(), &completed_threads));
		switch (add_task_result) {
		case max::Hardware::CPU::TaskQueue::AddTaskError::Okay:
			break;
		case max::Hardware::CPU::TaskQueue::AddTaskError::ShuttingDown:
		case max::Hardware::CPU::TaskQueue::AddTaskError::CouldNotSetEvent:
			return -1;
		}

		current_read_start.QuadPart += buffer_size;
		i++;
	}


	// Wait for all tasks to be complete.
	// TODO: Currently, the Shutdown is queued up before the follow-up tasks.
	max::Hardware::CPU::TaskRunnerLoop(main_task_queue->get());



	// Shutdown thread pool. Wait for the shutdown to complete.
	for (auto& thread: thread_pool->task_threads_) {
		thread.task_queue_->Shutdown();
	}
	for (auto& thread: thread_pool->task_threads_) {
		thread.thread_.join();
	}


	return 0;
}