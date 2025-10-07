// Copyright 2025, The FileReadSpeedTest Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FILEREADSPEEDTEST_WIN32TYPES_HPP
#define FILEREADSPEEDTEST_WIN32TYPES_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace FileReadSpeedTest {

	class GenericHandleObject {
	public:

		explicit GenericHandleObject(HANDLE handle) noexcept;
		GenericHandleObject(const GenericHandleObject& rhs) = delete;
		GenericHandleObject(GenericHandleObject&& rhs) noexcept;
		~GenericHandleObject() noexcept;

		GenericHandleObject& operator =(const GenericHandleObject& rhs) = delete;
		GenericHandleObject& operator =(GenericHandleObject&& rhs) noexcept;

		HANDLE handle_ = INVALID_HANDLE_VALUE;

	};

	class SpecificHandleObject : public GenericHandleObject {
	public:

		explicit SpecificHandleObject(HANDLE handle) noexcept;

	};

} // namespace FileReadSpeedTest

#endif // #ifndef FILEREADSPEEDTEST_WIN32TYPES_HPP