// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_GETPROCESSORCOREINFORMATION_HPP
#define FILEREADSPEEDTEST_GETPROCESSORCOREINFORMATION_HPP

#include <cstdint>
#include <optional>
#include <vector>

namespace FileReadSpeedTest {

	struct ProcessorCoreInformation {
	public:

		explicit ProcessorCoreInformation(uint16_t efficiency_class, uint16_t count, bool has_hyperthreading) noexcept;

		uint16_t efficiency_class_;
		uint16_t count_;
		bool has_hyperthreading_;
	};

	std::optional<std::vector<ProcessorCoreInformation>> GetProcessorCoreInformation() noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_GETPROCESSORCOREINFORMATION_HPP