// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP
#define FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP

#include <chrono>
#include <expected>
#include <memory>
#include <optional>
#include <string_view>
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

	using CompletionPort = SpecificHandleObject;

	struct OverlappedIOFile : public SpecificHandleObject {

		explicit OverlappedIOFile(HANDLE handle) noexcept
			: SpecificHandleObject(std::move(handle))
			, file_open_time_(std::chrono::high_resolution_clock::now())
		{}

		std::chrono::time_point<std::chrono::high_resolution_clock> file_open_time_;

	};

	std::optional<FileReadSpeedTest::OverlappedIOFile> CreateOverlappedIOFile(LPCSTR file_name) noexcept;

	struct IOContext {
		explicit IOContext(OVERLAPPED overlapped, HANDLE file_handle, OSAllocation buffer, DWORD bytes_to_read, DWORD file_offset) noexcept;

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

		explicit OverlappedIOFileRead(OverlappedIOFile overlapped_io_file, CompletionPort completion_port, std::vector<IOContext> contexts, LARGE_INTEGER file_size) noexcept;

		void Read(size_t context_index) noexcept;
		void WaitForThreadsToFinish() noexcept;

		void PrintResults() noexcept;

		OverlappedIOFile overlapped_io_file_;
		CompletionPort completion_port_;
		// TODO: Put contexts in their own cache line so there isn't cache contention
		std::vector<IOContext> contexts_;
		LARGE_INTEGER file_size_;
		std::chrono::time_point<std::chrono::high_resolution_clock> read_issue_time_;

	};

	std::optional<size_t> GetIdealBufferSize(const OverlappedIOFile& file) noexcept;

	enum PrepareToReadFileError {
		CouldNotOpenFile,
		CouldNotCreateCompletionPort,
		CouldNotCreateThread,
		CouldNotGetFileSize,
		CouldNotCreateIOContexts,
	};
	std::expected<OverlappedIOFileRead, PrepareToReadFileError> PrepareToReadFile(OverlappedIOFile& file, DWORD worker_thread_count, size_t buffer_size) noexcept;

	// Returns std::nullopt if the new offset would be past the end of the file.
	std::optional<LARGE_INTEGER> GetNewReadOffset(LARGE_INTEGER file_size, LARGE_INTEGER current_read_start, size_t buffer_interval_in_bytes) noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_OVERLAPPEDIOFILEREAD_HPP