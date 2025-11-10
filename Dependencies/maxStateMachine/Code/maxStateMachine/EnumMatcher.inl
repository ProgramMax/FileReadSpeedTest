#include <utility>

namespace maxStateMachine {

	template<typename T>
	constexpr EnumValue<T>::EnumValue(T value, std::string_view representation) noexcept
		: value_(std::move(value))
		, representation_(std::move(representation))
	{}

	template<typename T>
	constexpr EnumMatcher<T>::EnumMatcher(std::vector<EnumValue<T>> values) noexcept
		: values_(std::move(values))
	{}

} // namespace maxStateMachine