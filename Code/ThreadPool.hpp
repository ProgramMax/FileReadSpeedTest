// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_THREADPOOL_HPP
#define FILEREADSPEEDTEST_THREADPOOL_HPP

#include <optional>
#include <vector>

#include <max/Hardware/CPU/TaskThread.hpp>

namespace FileReadSpeedTest {

	class ThreadPool {
	public:

		explicit ThreadPool(std::vector<max::Hardware::CPU::TaskThread> task_threads) noexcept;

		std::vector<max::Hardware::CPU::TaskThread> task_threads_;

	};

	std::optional<ThreadPool> CreateThreadPool() noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_THREADPOOL_HPP