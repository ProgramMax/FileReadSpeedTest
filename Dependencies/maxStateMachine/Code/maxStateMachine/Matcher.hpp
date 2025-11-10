#ifndef MAXSTATEMACHINE_MATCHER_HPP
#define MAXSTATEMACHINE_MATCHER_HPP

namespace maxStateMachine {

	// TODO: Make Matcher concept

	template<typename MatcherType, typename InputType>
	bool DoesMatch(const MatcherType& matcher_type, const InputType& input_type) noexcept {
		return matcher_type.DoesMatch(input_type.value_);
	}

} // namespace maxStateMachine

#endif // #ifndef MAXSTATEMACHINE_MATCHER_HPP