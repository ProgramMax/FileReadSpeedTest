#ifndef MAXSTATEMACHINE_TRANSITION_HPP
#define MAXSTATEMACHINE_TRANSITION_HPP

#include <optional>
#include <utility>

namespace maxStateMachine {

	template<typename MatcherType, typename CallbackType>
	class Transition {
	public:

		constexpr explicit Transition(MatcherType matcher, CallbackType callback) noexcept
			: matcher_(std::move(matcher))
			, callback_(std::move(callback))
		{}

		template<typename T>
		constexpr std::optional<size_t> AttemptTransition(T input) noexcept {
			if (matcher_.DoesMatch(input)) {
				return callback_(std::move(input));
			}
			return std::nullopt;
		}

		MatcherType matcher_;
		CallbackType callback_;

	};

} // namespace maxStateMachine

#endif // #ifndef MAXSTATEMACHINE_TRANSITION_HPP