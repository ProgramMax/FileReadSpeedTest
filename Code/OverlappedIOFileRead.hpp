// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP
#define FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP

#include <chrono>
#include <expected>
#include <memory>
#include <optional>
#include <vector>
#include <utility>

#include "OSAllocator.hpp"
#include "Win32Types.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace FileReadSpeedTest {

	//using OverlappedIOFile = SpecificHandleObject;
	using CompletionPort = SpecificHandleObject;
	using Thread = SpecificHandleObject;

	struct OverlappedIOFile : public SpecificHandleObject {

		explicit OverlappedIOFile(HANDLE handle) noexcept
			: SpecificHandleObject(std::move(handle))
			, file_open_time_(std::chrono::high_resolution_clock::now())
		{}

		std::chrono::time_point<std::chrono::high_resolution_clock> file_open_time_;

	};

	class ThreadPool {
	public:

		explicit ThreadPool(std::vector<Thread> threads) noexcept;

		std::vector<Thread> threads_;

	};

	struct IOContext {
		IOContext(OVERLAPPED overlapped, HANDLE file_handle, OSAllocation buffer, DWORD bytes_to_read, DWORD file_offset) noexcept;

		OVERLAPPED overlapped_;
		HANDLE file_handle_;
		OSAllocation buffer_;
		//std::unique_ptr<char[]> buffer_;
		DWORD bytes_to_read_;
		DWORD file_offset_;
		bool is_complete_;
		std::chrono::time_point<std::chrono::high_resolution_clock> request_start_time_;
		std::chrono::time_point<std::chrono::high_resolution_clock> request_complete_time_;

	};

	class OverlappedIOFileRead {
	public:

		explicit OverlappedIOFileRead(OverlappedIOFile overlapped_io_file, CompletionPort completion_port, ThreadPool thread_pool, std::vector<IOContext> contexts) noexcept;

		void Read() noexcept;
		void WaitForThreadsToFinish() noexcept;

		OverlappedIOFile overlapped_io_file_;
		CompletionPort completion_port_;
		ThreadPool thread_pool_;
		// TODO: Put contexts in their own cache line so there isn't cache contention
		std::vector<IOContext> contexts_;
		std::chrono::time_point<std::chrono::high_resolution_clock> read_issue_time_;
	
	};

	enum PrepareToReadFileError {
		CouldNotOpenFile,
		CouldNotCreateCompletionPort,
		CouldNotCreateThread,
		CouldNotGetFileSize,
		CouldNotCreateIOContexts,
	};
	std::expected<OverlappedIOFileRead, PrepareToReadFileError> PrepareToReadFile(LPCSTR file_name, DWORD worker_thread_count) noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP