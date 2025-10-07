// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_GETPROCESSORINFORMATION_HPP
#define FILEREADSPEEDTEST_GETPROCESSORINFORMATION_HPP

#include <optional>

namespace FileReadSpeedTest {

	class ProcessorCoreInformation {
	public:
		int actual_cores_;
		int hyperthreading_cores_;
	};

	std::optional<ProcessorCoreInformation> GetProcessorInformation() noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_GETPROCESSORINFORMATION_HPP