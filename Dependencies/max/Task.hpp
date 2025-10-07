// Copyright 2025, The max Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAX_TASK_HPP
#define MAX_TASK_HPP

namespace max {

	class Task {
	public:

		virtual ~Task() noexcept;

		virtual void Run() noexcept = 0;

	};

} // namespace max

#endif // #ifndef MAX_TASK_HPP