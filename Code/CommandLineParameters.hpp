// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_COMMANDLINEPARAMETERS_HPP
#define FILEREADSPEEDTEST_COMMANDLINEPARAMETERS_HPP

#include <string>
#include <string_view>
#include <variant>

namespace FileReadSpeedTest {

	struct SuccessAction {
		std::string_view file_path_;
	};

	struct ErrorAction {
		std::string error_message_;
	};

	using Action = std::variant<SuccessAction, ErrorAction>;

	Action ProcessCommandLineParameters(int argc, const char * argv[]) noexcept;

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_COMMANDLINEPARAMETERS_HPP