namespace maxStateMachine {

	template<typename T>
	constexpr bool AnythingMatcher<T>::DoesMatch(const T& input) const noexcept {
		return true;
	}

} // namespace maxStateMachine