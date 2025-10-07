// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GetProcessorInformation.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <memory>

//#include <iostream>

namespace FileReadSpeedTest {

	/*struct ProcessorInformation {
		int actual_cores_;
		int hyperthreading_cores_;
	};*/

	std::optional<ProcessorCoreInformation> GetProcessorInformation() noexcept {
		DWORD required_buffer_size = 0;
		BOOL result = GetLogicalProcessorInformation(nullptr, &required_buffer_size);
		if (result == FALSE) {
			DWORD error_code = GetLastError();
			if (error_code != ERROR_INSUFFICIENT_BUFFER) {
				return std::nullopt;
			}
		}
		size_t processor_information_count = required_buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		auto processor_information = std::make_unique<SYSTEM_LOGICAL_PROCESSOR_INFORMATION[]>(processor_information_count);
		result = GetLogicalProcessorInformation(processor_information.get(), &required_buffer_size);
		if (result == FALSE) {
			return std::nullopt;
		}

		ProcessorCoreInformation processor_core_information = {};

		for(DWORD i = 0; i < processor_information_count; i++) {
			//std::cout << "Core " << i << ": " << processor_information[i].ProcessorMask << std::endl;
			switch (processor_information[i].Relationship) {
			/*
			case RelationProcessorPackage:
				std::cout << "\tPhysical package" << std::endl;
				break;
			case RelationNumaNode:
				std::cout << "\tNUMA node: " << processor_information[i].NumaNode.NodeNumber << std::endl;
				break;
			*/
			case RelationProcessorCore:
			{
				processor_core_information.actual_cores_++;
				if (processor_information[i].ProcessorCore.Flags == 1) {
					processor_core_information.hyperthreading_cores_++;
				}
			}
			break;
			/*
			case RelationCache:
				std::cout << "\tL";
				switch (processor_information[i].Cache.Level) {
				case 1:
					std::cout << "1 ";
					break;
				case 2:
					std::cout << "2 ";
					break;
				case 3:
					std::cout << "3 ";
					break;
				}
				switch (processor_information[i].Cache.Type) {
				case CacheUnified:
					std::cout << "unified";
					break;
				case CacheInstruction:
					std::cout << "instuction";
					break;
				case CacheData:
					std::cout << "data";
					break;
				case CacheTrace:
					std::cout << "trace";
					break;
				}
				std::cout << " cache, " << processor_information[i].Cache.Size << " KiB" << std::endl;
				std::cout << "\tCache line size: " << processor_information[i].Cache.LineSize << " KiB" << std::endl;
				break;
			*/
			}
		}

		return processor_core_information;
	}

} // namespace FileReadSpeedTest