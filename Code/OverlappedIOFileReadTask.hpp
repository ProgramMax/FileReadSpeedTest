// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OVERLAPPEDIOFILEREADTASK_HPP
#define OVERLAPPEDIOFILEREADTASK_HPP

#include <expected>
#include <filesystem>
#include <functional>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <max/Task.hpp>
#include <max/TaskQueue.hpp>

#include "OverlappedIOFileRead.hpp"

namespace FileReadSpeedTest {

	class OverlappedIOFileReadTask : public max::Task {
	public:

		//explicit OverlappedIOFileReadTask(HANDLE completion_port, size_t buffer_interval_in_bytes, LARGE_INTEGER file_size) noexcept;
		//explicit OverlappedIOFileReadTask(OverlappedIOFileRead* overlapped_io_file_read) noexcept;
		explicit OverlappedIOFileReadTask(OverlappedIOFileRead* overlapped_io_file_read, LARGE_INTEGER current_read_start, size_t buffer_interval_in_bytes, size_t context_index, size_t thread_count, max::TaskQueue* task_queue, max::TaskQueue* max_task_queue, size_t* completed_threads) noexcept;

		void Run() noexcept override;

	private:

		//HANDLE completion_port_;
		size_t buffer_interval_in_bytes_;
		LARGE_INTEGER current_read_start_;
		size_t context_index_;
		size_t thread_count_;
		max::TaskQueue* task_queue_;
		max::TaskQueue* main_task_queue_;
		size_t* completed_threads_;

		OverlappedIOFileRead* overlapped_io_file_read_;

	};

	class Finished : public max::Task {
	public:

		explicit Finished(size_t* completed_threads, size_t thread_count, max::TaskQueue* main_task_queue) noexcept;

		void Run() noexcept override;

		size_t* completed_threads_;
		size_t thread_count_;
		max::TaskQueue* main_task_queue_;

	};

} // namespace FileReadSpeedTest

#endif // #ifndef OVERLAPPEDIOFILEREADTASK_HPP