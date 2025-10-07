// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "GetProcessorInformation.hpp"
#include "OverlappedIOFileRead.hpp"

int main(int argc, const char * argv[]) {
	if (argc != 2) {
		std::cerr << "Error: No file provided.\n";
		std::cerr << "Use the form: FileReadSpeedTest.exe C:\\my\\file.txt" << std::endl;
		return -1;
	}

	auto processor_information = FileReadSpeedTest::GetProcessorInformation();
	if (!processor_information.has_value()) {
		std::cerr << "Error: Could not find processor information." << std::endl;
		return -1;
	}

	std::cout << "CPU cores: " << processor_information->actual_cores_;
	if (processor_information->hyperthreading_cores_ != 0) {
		std::cout << " (and " << processor_information->hyperthreading_cores_ << " hyperthreading cores)";
	}
	std::cout << std::endl;

	auto overlapped_io_file_read = FileReadSpeedTest::PrepareToReadFile(argv[1], processor_information->actual_cores_);
	if (!overlapped_io_file_read.has_value()) {
		std::cerr << "Error";
		return -1;
	}
	overlapped_io_file_read->Read();
	overlapped_io_file_read->WaitForThreadsToFinish();

	return 0;
}