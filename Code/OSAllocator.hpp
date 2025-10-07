// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OSALLOCATOR_HPP
#define OSALLOCATOR_HPP

#include <optional>

class OSAllocation {
public:

	explicit OSAllocation(void* memory) noexcept;

	void* memory_;

};

class OSAllocator {
public:

	static std::optional<OSAllocation> allocate(size_t size) noexcept;
	static void deallocate(OSAllocation& allocation) noexcept;

};

#endif // #ifndef OSALLOCATOR_HPP