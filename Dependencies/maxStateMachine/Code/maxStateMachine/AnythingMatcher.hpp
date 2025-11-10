#ifndef MAXSTATEMACHINE_ANYTHINGMATCHER_HPP
#define MAXSTATEMACHINE_ANYTHINGMATCHER_HPP

namespace maxStateMachine {

	template<typename T>
	class AnythingMatcher {
	public:

		using parameter_type = T;

		constexpr bool DoesMatch(const T& input) const noexcept;

		T value_ = {}; // TODO: I don't want this. I couldn't get parameter_type to work for me.

	};

} // namespace maxStateMachine

#include <maxStateMachine/AnythingMatcher.inl>

#endif // #ifndef MAXSTATEMACHINE_ANYTHINGMATCHER_HPP