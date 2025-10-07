// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GetProcessorCoreInformation.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <memory>
#include <utility>

ProcessorCoreInformation::ProcessorCoreInformation(uint16_t efficiency_class, uint16_t count, bool has_hyperthreading) noexcept
	: efficiency_class_(std::move(efficiency_class))
	, count_(std::move(count))
	, has_hyperthreading_(std::move(has_hyperthreading))
{}

std::optional<std::vector<ProcessorCoreInformation>> GetProcessorCoreInformation() noexcept {
	DWORD required_buffer_size = 0;
	BOOL result = GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &required_buffer_size);
	if (result == FALSE) {
		DWORD error_code = GetLastError();
		if (error_code != ERROR_INSUFFICIENT_BUFFER) {
			return std::nullopt;
		}
	}

	const size_t element_size = sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
	// char[] because chars enable aliasing
	auto buffer = std::make_unique<char[]>(required_buffer_size);
	auto buffer_end = buffer.get() + required_buffer_size;
	auto* current_pointer = buffer.get();
	auto* processor_information = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)current_pointer;
	result = GetLogicalProcessorInformationEx(RelationProcessorCore, processor_information, &required_buffer_size);
	if (result == FALSE) {
		return std::nullopt;
	}

	std::vector<ProcessorCoreInformation> core_informations;
	for (size_t i = 0; (char*)current_pointer < buffer_end; i++) {
		uint32_t efficiency = processor_information->Processor.EfficiencyClass;
		bool hyperthreading = processor_information->Processor.Flags == LTP_PC_SMT;

		bool match_found = false;
		for (auto& core_information : core_informations) {
			if (core_information.efficiency_class_ == efficiency &&
			    core_information.has_hyperthreading_ == hyperthreading) {
				match_found = true;
				core_information.count_++;
			}
		}
		if (!match_found) {
			core_informations.emplace_back(efficiency, 1, hyperthreading);
		}

		current_pointer += processor_information->Size;
		processor_information = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)current_pointer;
	}

	return core_informations;
}