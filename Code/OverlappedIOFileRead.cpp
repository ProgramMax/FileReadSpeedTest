// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OverlappedIOFileRead.hpp"

#include <optional>

#include <iostream>
#include <utility>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace {

	std::chrono::time_point<std::chrono::high_resolution_clock> pre_open_time_;

	std::optional<FileReadSpeedTest::CompletionPort> CreateCompletionPort(FileReadSpeedTest::OverlappedIOFile& overlapped_io_file, DWORD worker_thread_count) noexcept {
		HANDLE completion_port_handle = CreateIoCompletionPort(overlapped_io_file.handle_, NULL, 0, worker_thread_count);
		if (completion_port_handle == NULL) {
			// GetLastError
			return std::nullopt;
		}

		return FileReadSpeedTest::CompletionPort{std::move(completion_port_handle)};
	}

	std::optional<std::vector<FileReadSpeedTest::IOContext>> CreateIOContexts(LARGE_INTEGER file_size, unsigned long buffer_size, FileReadSpeedTest::OverlappedIOFile& overlapped_io_file) noexcept {
		DWORD operation_count = (DWORD)((file_size.QuadPart + buffer_size - 1) / buffer_size);

		std::vector<FileReadSpeedTest::IOContext> contexts;
		for (DWORD i = 0; i < operation_count; ++i) {
			DWORD file_offset = i * buffer_size;

			OVERLAPPED overlapped;
			ZeroMemory(&overlapped, sizeof(overlapped));
			overlapped.Offset = file_offset;
			overlapped.OffsetHigh = 0;

			auto allocation = FileReadSpeedTest::OSAllocator::allocate(buffer_size);
			if (!allocation.has_value()) {
				return std::nullopt;
			}
			// TODO: Confirm the allocation is aligned to volume sector size
			// TODO: Confirm buffer_size is a multiple of volume sector size

			auto io_context = FileReadSpeedTest::IOContext{std::move(overlapped), overlapped_io_file.handle_, std::move(*allocation), buffer_size, file_offset};

			contexts.emplace_back(std::move(io_context));
		}

		return contexts;
	}

	DWORD WINAPI WorkerThread(LPVOID param) {
		HANDLE completion_port = static_cast<HANDLE>(param);
		DWORD bytes_transferred;
		ULONG_PTR completion_key;
		FileReadSpeedTest::IOContext* context = nullptr;

		while (true) {
			BOOL result = GetQueuedCompletionStatus(completion_port, &bytes_transferred, &completion_key, reinterpret_cast<LPOVERLAPPED*>(&context), INFINITE);

			// Check for shutdown signal
			if (completion_key == 0 && context == nullptr) {
				break;
			}

			if (!result) {
				DWORD error = GetLastError();
				if (error != ERROR_HANDLE_EOF) {
					// TODO: error
				}
				context->is_complete_ = true;
				continue;
			}

			context->request_complete_time_ = std::chrono::high_resolution_clock::now();
			context->is_complete_ = true;
		}

		return 0;
	}

} // anonymous namespace

namespace FileReadSpeedTest {

	std::optional<FileReadSpeedTest::OverlappedIOFile> CreateOverlappedIOFile(LPCSTR file_name) noexcept {
		pre_open_time_ = std::chrono::high_resolution_clock::now();
		// Use FILE_FLAG_RANDOM_ACCESS instead of FILE_FLAG_SEQUENTIAL_SCAN for file formats where that is a better fit
		// FILE_FLAG_NO_BUFFERING
		HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_NO_BUFFERING, NULL);
		if (file_handle == INVALID_HANDLE_VALUE) {
			// GetLastError
			return std::nullopt;
		}

		return FileReadSpeedTest::OverlappedIOFile{std::move(file_handle)};
	}

	IOContext::IOContext(OVERLAPPED overlapped, HANDLE file_handle, OSAllocation buffer, DWORD bytes_to_read, DWORD file_offset) noexcept
		: overlapped_(std::move(overlapped))
		, file_handle_(std::move(file_handle))
		, buffer_(std::move(buffer))
		, bytes_to_read_(std::move(bytes_to_read))
		, file_offset_(std::move(file_offset))
		, is_complete_(false)
	{}

	OverlappedIOFileRead::OverlappedIOFileRead(OverlappedIOFile overlapped_io_file, CompletionPort completion_port, std::vector<IOContext> contexts, LARGE_INTEGER file_size) noexcept
		: overlapped_io_file_(std::move(overlapped_io_file))
		, completion_port_(std::move(completion_port))
		, contexts_(std::move(contexts))
		, file_size_(std::move(file_size))
	{}

	void OverlappedIOFileRead::Read(size_t context_index) noexcept {
		contexts_[context_index].request_start_time_ = std::chrono::high_resolution_clock::now();
		BOOL result = ReadFile(overlapped_io_file_.handle_, contexts_[context_index].buffer_.memory_, contexts_[context_index].bytes_to_read_, nullptr, &contexts_[context_index].overlapped_);
		if (!result) {
			DWORD error = GetLastError();
			// TODO: ERROR_INVALID_USER_BUFFER or ERROR_NOT_ENOUGH_MEMORY if too many outstanding asynchronous IO operations
			if (error != ERROR_IO_PENDING) {
				// TODO: error
			}
		}

		// TODO: Handle when the OS makes the operation synchronous:
		// https://learn.microsoft.com/en-us/previous-versions/troubleshoot/windows/win32/asynchronous-disk-io-synchronous
		// This can happen when FS compression or encryption is enabled.
	}

	void OverlappedIOFileRead::WaitForThreadsToFinish() noexcept {
		// Wait for all operations to complete
		bool all_complete = false;
		while (!all_complete) {
			all_complete = true;
			for (const auto& context : contexts_) {
				if (!context.is_complete_) {
					all_complete = false;
					break;
				}
			}
			// There is a much better way to handle this. But for now, whatever.
			Sleep(100);
		}


		// Signal worker threads to shutdown
		/*
		for (size_t i = 0; i < thread_pool_.threads_.size(); ++i) {
			PostQueuedCompletionStatus(completion_port_.handle_, 0, 0, nullptr);
		}

		// Wait for worker threads to finish
		WaitForMultipleObjects(static_cast<DWORD>(thread_pool_.threads_.size()), (const HANDLE*)thread_pool_.threads_.data(), TRUE, INFINITE);
		*/
		PrintResults();
	}

	void OverlappedIOFileRead::PrintResults() noexcept {
		auto open_delay = std::chrono::duration_cast<std::chrono::nanoseconds>(overlapped_io_file_.file_open_time_ - pre_open_time_);
		auto open_to_read_delay = std::chrono::duration_cast<std::chrono::nanoseconds>(read_issue_time_ - overlapped_io_file_.file_open_time_);
		std::cout << "Open delay: " << open_delay << std::endl;
		std::cout << "Open to read delay: " << open_to_read_delay << std::endl;

		size_t i = 0;
		for (auto& context : contexts_) {
			auto read_issue_delay = std::chrono::duration_cast<std::chrono::nanoseconds>(context.request_start_time_ - read_issue_time_);
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(context.request_complete_time_ - context.request_start_time_);

			std::cout << "Buffer " << i++ << " - Read issue delay: " << read_issue_delay << " - Issue to completion delay: " << duration << std::endl;

			OSAllocator::deallocate(context.buffer_);
		}
	}

	std::optional<size_t> GetIdealBufferSize(const OverlappedIOFile& file) noexcept {
		FILE_STORAGE_INFO file_storage_info = {};
		BOOL result = GetFileInformationByHandleEx(file.handle_, FileStorageInfo, &file_storage_info, sizeof(file_storage_info));
		if (result == FALSE) {
			return std::nullopt;
		}
		unsigned long partition_block_size = file_storage_info.LogicalBytesPerSector;
		// When doing writes instead of reads, prefer file_storage_info.PhysicalBytesPerSectorForPerformance;

		// Get the page size
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		/*if (compiling for x86 or arm64) {
		SYSTEM_INFO native_system_info;
		GetNativeSystemInfo(&native_system_info);
		}*/
		partition_block_size = system_info.dwPageSize;
		// TODO: Confirm the page size is a multiple of the logical bytes per sector

		// On Windows, VirtualAlloc has 64 KiB granularity:
		// https://devblogs.microsoft.com/oldnewthing/20031008-00/?p=42223
		// TODO: Should we push the buffer size to 64 KiB?

		return partition_block_size;
	}


	std::expected<OverlappedIOFileRead, PrepareToReadFileError> PrepareToReadFile(OverlappedIOFile& file, DWORD worker_thread_count, size_t buffer_size) noexcept {
		// TODO: Print drive make & model
		// TODO: Print filesystem info (format, block size)

		LARGE_INTEGER file_size;
		BOOL result = GetFileSizeEx(file.handle_, &file_size);
		if (result == 0) {
			// GetLastError
			return std::unexpected{PrepareToReadFileError::CouldNotGetFileSize};
		}

		std::cout << "File size: " << file_size.QuadPart << std::endl;


		auto completion_port = CreateCompletionPort(file, worker_thread_count);
		if (!completion_port.has_value()) {
			return std::unexpected{PrepareToReadFileError::CouldNotCreateCompletionPort};
		}

		// TODO: Creating the threads takes a significant amount of time.
		// This happens after the file is opened and the completion port is created.
		// The OS *could* begin prefetching the data. I don't know if it does or not.
		// But if it does, this gives it ample opportunity to impact the measurements.
		/*
		auto thread_pool = CreateThreadPool(WorkerThread, completion_port->handle_, worker_thread_count);
		if (!thread_pool.has_value()) {
			return std::unexpected{PrepareToReadFileError::CouldNotCreateThread};
		}
		*/

		auto io_contexts = CreateIOContexts(file_size, buffer_size, file);
		if (!io_contexts.has_value()) {
			return std::unexpected{PrepareToReadFileError::CouldNotCreateIOContexts};
		}

		return OverlappedIOFileRead(std::move(file),
		                            std::move(*completion_port),
		                            std::move(*io_contexts),
		                            std::move(file_size));
	}

	std::optional<LARGE_INTEGER> GetNewReadOffset(LARGE_INTEGER file_size, LARGE_INTEGER current_read_start, size_t buffer_interval_in_bytes) noexcept {
		LARGE_INTEGER new_read_offset = {};
		new_read_offset.QuadPart = current_read_start.QuadPart + buffer_interval_in_bytes;

		if (new_read_offset.QuadPart > file_size.QuadPart) {
			// Past the end of the file
			return std::nullopt;
		}

		return new_read_offset;
	}

} // namespace FileReadSpeedTest 