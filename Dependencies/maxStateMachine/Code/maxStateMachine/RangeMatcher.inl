#include <utility>

namespace maxStateMachine {

	template<typename T>
	constexpr RangeMatcher<T>::RangeMatcher(T lower_bound, T upper_bound) noexcept
		: lower_bound_(std::move(lower_bound))
		, upper_bound_(std::move(upper_bound))
	{}

	template<typename T>
	constexpr bool RangeMatcher<T>::DoesMatch(const T& input) const noexcept {
		return lower_bound_ <= input && input <= upper_bound_;
	}

} // namespace maxStateMachine