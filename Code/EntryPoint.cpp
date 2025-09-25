#include <iostream>

#include "OverlappedIOFileRead.hpp"

int main(int argc, const char * argv[]) {
	if (argc != 2) {
		std::cerr << "Error: No file provided.\n";
		std::cerr << "Use the form: FileReadSpeedTest.exe C:\\my\\file.txt" << std::endl;
		return -1;
	}

	const DWORD worker_thread_count = 6;
	// TODO: Take the file as part of the command line parameters.
	auto overlapped_io_file_read = PrepareToReadFile(argv[1], worker_thread_count);
	if (!overlapped_io_file_read.has_value()) {
		std::cerr << "Error";
		return -1;
	}
	overlapped_io_file_read->Read();
	overlapped_io_file_read->WaitForThreadsToFinish();

	return 0;
}