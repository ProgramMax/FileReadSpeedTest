#include <cstdint>
#include <iostream>

#include "GetProcessorCoreInformation.hpp"
#include "OverlappedIOFileRead.hpp"

int main(int argc, const char * argv[]) {
	if (argc != 2) {
		std::cerr << "Error: No file provided.\n";
		std::cerr << "Use the form: FileReadSpeedTest.exe C:\\my\\file.txt" << std::endl;
		return -1;
	}

	auto processor_core_information = GetProcessorCoreInformation();
	if (!processor_core_information.has_value()) {
		std::cerr << "Error: Could not find processor information." << std::endl;
		return -1;
	}

	int16_t max_efficiency = -1;
	uint16_t max_efficiency_core_count = 0;
	for (auto& core_information : *processor_core_information) {
		std::cout << "CPU cores: " << core_information.count_ << " @ performance class: " << core_information.efficiency_class_;
		if (core_information.has_hyperthreading_ != 0) {
			std::cout << " (with hyperthreading)";
		}

		if (core_information.efficiency_class_ > max_efficiency) {
			max_efficiency = core_information.efficiency_class_;
			max_efficiency_core_count = core_information.count_;
		}
		std::cout << std::endl;
	}

	auto overlapped_io_file_read = PrepareToReadFile(argv[1], max_efficiency_core_count);
	if (!overlapped_io_file_read.has_value()) {
		std::cerr << "Error";
		return -1;
	}
	overlapped_io_file_read->Read();
	overlapped_io_file_read->WaitForThreadsToFinish();

	return 0;
}