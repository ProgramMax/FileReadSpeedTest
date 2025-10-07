// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GETPROCESSORINFORMATION_HPP
#define GETPROCESSORINFORMATION_HPP

#include <optional>

class ProcessorCoreInformation {
public:
	int actual_cores_;
	int hyperthreading_cores_;
};

std::optional<ProcessorCoreInformation> GetProcessorInformation() noexcept;

#endif // #ifndef GETPROCESSORINFORMATION_HPP