#ifndef MAXSTATEMACHINE_STRINGINPUT_HPP
#define MAXSTATEMACHINE_STRINGINPUT_HPP

#include <string_view>
#include <utility>

namespace maxStateMachine {

	class StringInput {
	public:

		constexpr explicit StringInput(std::string_view value) noexcept
			: value_(std::move(value))
		{}

		std::string_view value_;

	};

} // namespace maxStateMachine

#endif // #ifndef MAXSTATEMACHINE_STRINGINPUT_HPP