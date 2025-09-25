#include <iostream>

#include "OverlappedIOFileRead.hpp"

int main() {
	const DWORD worker_thread_count = 6;
	// TODO: Take the file as part of the command line parameters.
	auto overlapped_io_file_read = PrepareToReadFile(R"(../../Test Data/crystaldiskmark nvme ssd.png)", worker_thread_count);
	if (!overlapped_io_file_read.has_value()) {
		std::cerr << "Error";
		return -1;
	}
	overlapped_io_file_read->Read();
	overlapped_io_file_read->WaitForThreadsToFinish();

	return 0;
}