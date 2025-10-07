// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_OSALLOCATOR_HPP
#define FILEREADSPEEDTEST_OSALLOCATOR_HPP

#include <optional>

namespace FileReadSpeedTest {

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

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_OSALLOCATOR_HPP