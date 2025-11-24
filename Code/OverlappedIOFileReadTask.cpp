// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OverlappedIOFileReadTask.hpp"

#include <memory>
#include <utility>

#include <iostream>

namespace FileReadSpeedTest {

	/*
	OverlappedIOFileReadTask::OverlappedIOFileReadTask(HANDLE completion_port, size_t buffer_interval_in_bytes, LARGE_INTEGER file_size) noexcept
		: completion_port_(std::move(completion_port))
		, buffer_interval_in_bytes_(std::move(buffer_interval_in_bytes))
		, file_size_(std::move(file_size))
	{}
	*/
	/*
	OverlappedIOFileReadTask::OverlappedIOFileReadTask(OverlappedIOFileRead* overlapped_io_file_read) noexcept
		: overlapped_io_file_read_(std::move(overlapped_io_file_read))
	{}
	*/
	OverlappedIOFileReadTask::OverlappedIOFileReadTask(OverlappedIOFileRead* overlapped_io_file_read, LARGE_INTEGER current_read_start, size_t buffer_interval_in_bytes, size_t context_index, size_t thread_count, max::Hardware::CPU::TaskQueue* task_queue, max::Hardware::CPU::TaskQueue* main_task_queue, size_t* completed_threads) noexcept
		: overlapped_io_file_read_(std::move(overlapped_io_file_read))
		, current_read_start_(std::move(current_read_start))
		, buffer_interval_in_bytes_(std::move(buffer_interval_in_bytes))
		, context_index_(std::move(context_index))
		, thread_count_(std::move(thread_count))
		, task_queue_(std::move(task_queue))
		, main_task_queue_(std::move(main_task_queue))
		, completed_threads_(std::move(completed_threads))
	{}

	void OverlappedIOFileReadTask::Run() noexcept {
		DWORD bytes_transferred;
		ULONG_PTR completion_key;
		FileReadSpeedTest::IOContext* context = nullptr;

		BOOL result = GetQueuedCompletionStatus(overlapped_io_file_read_->completion_port_.handle_, &bytes_transferred, &completion_key, reinterpret_cast<LPOVERLAPPED*>(&context), INFINITE);

		context->request_complete_time_ = std::chrono::high_resolution_clock::now();
		context->is_complete_ = true;

		auto new_read_offset = GetNewReadOffset(overlapped_io_file_read_->file_size_, /* TODO: use context->file_offset_ */ current_read_start_, buffer_interval_in_bytes_);
		if (!new_read_offset.has_value()) {
			// The new read offset is past the end of the file.
			// Do not issue a read.
			// Instead, notify the main task queue that this thread is done.
			auto add_task_result = main_task_queue_->AddTask(std::make_unique<Finished>(completed_threads_, thread_count_, main_task_queue_));
			switch (add_task_result) {
			case max::Hardware::CPU::TaskQueue::AddTaskError::Okay:
				break;
			case max::Hardware::CPU::TaskQueue::AddTaskError::ShuttingDown:
			case max::Hardware::CPU::TaskQueue::AddTaskError::CouldNotSetEvent:
				return;
			}

			return;
		}




		size_t new_context_index = context_index_ + thread_count_;


		auto add_task_result = task_queue_->AddTask(std::make_unique<OverlappedIOFileReadTask>(overlapped_io_file_read_, *new_read_offset, buffer_interval_in_bytes_, new_context_index, thread_count_, task_queue_, main_task_queue_, completed_threads_));
		switch (add_task_result) {
		case max::Hardware::CPU::TaskQueue::AddTaskError::Okay:
			break;
		case max::Hardware::CPU::TaskQueue::AddTaskError::ShuttingDown:
		case max::Hardware::CPU::TaskQueue::AddTaskError::CouldNotSetEvent:
			return;
		}






		// After the previous read has completed, issue the next read.
		/*
		result = ReadFile(overlapped_io_file_read_->overlapped_io_file_.handle_, overlapped_io_file_read_->contexts_[new_context_index].buffer_.memory_, overlapped_io_file_read_->contexts_[new_context_index].bytes_to_read_, nullptr, &overlapped_io_file_read_->contexts_[new_context_index].overlapped_);
		if (!result) {
			DWORD error = GetLastError();
			// TODO: ERROR_INVALID_USER_BUFFER or ERROR_NOT_ENOUGH_MEMORY if too many outstanding asynchronous IO operations
			if (error != ERROR_IO_PENDING) {
				// TODO: error
				return;
			}
		}
		*/

		// TODO: Handle when the OS makes the operation synchronous:
		// https://learn.microsoft.com/en-us/previous-versions/troubleshoot/windows/win32/asynchronous-disk-io-synchronous
		// This can happen when FS compression or encryption is enabled.


	}

	Finished::Finished(size_t* completed_threads, size_t thread_count, max::Hardware::CPU::TaskQueue* main_task_queue) noexcept
		: completed_threads_(std::move(completed_threads))
		, thread_count_(std::move(thread_count))
		, main_task_queue_(std::move(main_task_queue))
	{}

	void Finished::Run() noexcept {
		(*completed_threads_)++;

		if (*completed_threads_ == thread_count_) {
			main_task_queue_->Shutdown();
		}
	}

} // namespace FileReadSpeedTest