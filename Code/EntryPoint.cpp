// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <tuple>
#include <utility>
#include <variant>

#include "ThreadPool.hpp"
#include "OverlappedIOFileRead.hpp"
#include "OverlappedIOFileReadTask.hpp"
#include "CommandLineParameters.hpp"

namespace {

	struct ActionExecutor {

		void operator()(const FileReadSpeedTest::SuccessAction& action) const noexcept {
			// Do nothing. Continue execution.
		}

		void operator()(const FileReadSpeedTest::ErrorAction& action) const noexcept {
			std::cerr << action.error_message_;
			std::exit(-1);
		}

	};

} // anonymous namespace

int main(int argc, const char * argv[]) {
	auto action = FileReadSpeedTest::ProcessCommandLineParameters(argc, argv);
	std::visit(ActionExecutor{}, action);
	auto input_file = std::move(std::get<FileReadSpeedTest::SuccessAction>(action).file_path_);



	// Notify user of clock stability.
	if (std::chrono::high_resolution_clock::is_steady) {
		std::cout << "Clock is steady" << std::endl;
	} else {
		std::cout << "Clock is not steady" << std::endl;
	}



	// Create thread pool.
	auto thread_pool = std::optional<FileReadSpeedTest::ThreadPool>{std::nullopt};
	auto command_line_thread_count = std::get<FileReadSpeedTest::SuccessAction>(action).thread_count_;
	if (command_line_thread_count.has_value()) {
		thread_pool = FileReadSpeedTest::CreateThreadPool(*command_line_thread_count);
	} else {
		// Some workloads are inherently serial and may not benefit from additional threads.
		// Workloads that may benefit could require heavy or light processing.
		// All of this determines the type and quantity of cores to be used.
		thread_pool = FileReadSpeedTest::CreateThreadPool();
	}
	if (!thread_pool.has_value()) {
		std::cerr << "Could not create thread pool\n";
		return -1;
	}
	size_t worker_thread_count = thread_pool->task_threads_.size();
	std::cout << "Worker thread count: " << worker_thread_count << std::endl;






	// Open the file.
	auto file = FileReadSpeedTest::CreateOverlappedIOFile(input_file.data());
	if (!file.has_value()) {
		std::cerr << "Error opening file\n";
		return -1;
	}


	// TODO: Anecdotally, it seems like a smaller buffer 0 gets the initial response faster.
	// It might be good for buffer 0 to be smaller (512 bytes?) just to start the ball rolling while
	// buffer 1+ are the ideal throughput size.
	// Test this.
	auto buffer_size = size_t{0};
	auto command_line_buffer_size = std::get<FileReadSpeedTest::SuccessAction>(action).buffer_size_;
	if (command_line_buffer_size.has_value()) {
		buffer_size = *command_line_buffer_size;
		std::cout << "Using supplied buffer size: " << buffer_size << std::endl;
	} else {
		auto buffer_size_optional = GetIdealBufferSize(*file);
		if (!buffer_size_optional.has_value()) {
			std::cerr << "Error finding ideal buffer size\n";
			return -1;
		}

		buffer_size = *buffer_size_optional;
		std::cout << "Using calculated ideal buffer size: " << buffer_size << std::endl;
	}



	// Get queue depth
	auto queue_depth = size_t{0};
	auto command_line_queue_depth = std::get<FileReadSpeedTest::SuccessAction>(action).queue_depth_;
	if (command_line_queue_depth.has_value()) {
		queue_depth = *command_line_queue_depth;
		std::cout << "Using supplied queue_depth: " << queue_depth << std::endl;
	} else {
		// TODO: There is probably an ideal queue depth.
		// Research this.
		queue_depth = thread_pool->task_threads_.size();
		std::cout << "Using queue depth = thread count: " << queue_depth << std::endl;
	}



	// Prepare to read the file.
	auto overlapped_io_file_read = FileReadSpeedTest::PrepareToReadFile(*file, worker_thread_count, buffer_size);
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
	


	// Add initial read tasks to the thread pool
	size_t thread_count = thread_pool->task_threads_.size();
	LARGE_INTEGER current_read_start = {};
	auto i = size_t{0};
	for (auto& thread : thread_pool->task_threads_) {
		auto add_task_result = thread.task_queue_->AddTask(std::make_unique<FileReadSpeedTest::OverlappedIOFileReadTask>(&overlapped_io_file_read.value(), current_read_start, queue_depth * buffer_size, i, thread_count, queue_depth, thread.task_queue_.get(), main_task_queue->get(), &completed_threads));
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



	// Issue initial reads.
	overlapped_io_file_read->read_issue_time_ = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < queue_depth; i++) {
		overlapped_io_file_read->Read(i);
	}



	// Wait for all tasks to be complete.
	// TODO: Currently, the Shutdown is queued up before the follow-up tasks.
	max::Hardware::CPU::TaskRunnerLoop(main_task_queue->get());



	// TODO: If queue depth is less than thread pool size, some threads will always be waiting.
	// Shutdown thread pool. Wait for the shutdown to complete.
	for (auto& thread: thread_pool->task_threads_) {
		thread.task_queue_->Shutdown();
	}
	for (auto& thread: thread_pool->task_threads_) {
		thread.thread_.join();
	}



	// TODO: There seems to be some sort of race condition when queue depth != thread count.
	// Some of the times aren't updated away from 0.
	// This might also be context[i]->time is set on the wrong i.
	overlapped_io_file_read->PrintResults();


	return 0;
}