// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OSAllocator.hpp"

#include <utility>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

OSAllocation::OSAllocation(void* memory) noexcept
	: memory_(std::move(memory))
{}


std::optional<OSAllocation> OSAllocator::allocate(size_t size) noexcept {
	void* pointer = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pointer == NULL) {
		// GetLastError()
		return std::nullopt;
	}

	return OSAllocation{pointer};
}

void OSAllocator::deallocate(OSAllocation& allocation) noexcept {
	if (allocation.memory_) {
		BOOL result = VirtualFree(allocation.memory_, 0, MEM_RELEASE);
		if (result == 0) {
			// GetLastError()
		}

		allocation.memory_ = nullptr;
	}
}