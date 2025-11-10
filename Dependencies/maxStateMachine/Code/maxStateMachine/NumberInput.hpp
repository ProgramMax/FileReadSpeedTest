#ifndef MAXSTATEMACHINE_NUMBERINPUT_HPP
#define MAXSTATEMACHINE_NUMBERINPUT_HPP

namespace maxStateMachine {

	template<typename T>
	class NumberInput {
	public:

		constexpr explicit NumberInput(T value) noexcept;

		T value_;

	};

} // namespace maxStateMachine

#include <maxStateMachine/NumberInput.inl>

#endif // #ifndef MAXSTATEMACHINE_NUMBERINPUT_HPP