#ifndef MAXSTATEMACHINE_STRINGMATCHER_HPP
#define MAXSTATEMACHINE_STRINGMATCHER_HPP

#include <string_view>
#include <utility>

namespace maxStateMachine {

	class StringMatcher {
	public:

		using parameter_type = std::string_view;

		constexpr explicit StringMatcher(std::string_view value) noexcept
			: value_(std::move(value))
		{}

		constexpr bool DoesMatch(const std::string_view& input) const noexcept {
			return value_ == input;
		}

		std::string_view value_;

	};

} // namespace maxStateMachine

#endif // #ifndef MAXSTATEMACHINE_STRINGMATCHER_HPP