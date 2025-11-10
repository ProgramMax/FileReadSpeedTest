#ifndef MAXSTATEMACHINE_RANGE_HPP
#define MAXSTATEMACHINE_RANGE_HPP

namespace maxStateMachine {

	template<typename T>
	class RangeMatcher {
	public:

		using parameter_type = T;

		constexpr explicit RangeMatcher(T lower_bound, T upper_bound) noexcept;

		constexpr bool DoesMatch(const T& input) const noexcept;

		T value_ = {}; // TODO: I don't want this. I couldn't get parameter_type to work for me.
		T lower_bound_;
		T upper_bound_;

	};

} // namespace maxStateMachine

#include <maxStateMachine/RangeMatcher.inl>

#endif // #ifndef MAXSTATEMACHINE_RANGE_HPP