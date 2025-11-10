#include <utility>

namespace maxStateMachine {

	template<typename T>
	constexpr NumberInput<T>::NumberInput(T value) noexcept
		: value_(std::move(value))
	{}

} // namespace maxStateMachine