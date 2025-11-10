#ifndef MAXSTATEMACHINE_ENUMMATCHER_HPP
#define MAXSTATEMACHINE_ENUMMATCHER_HPP

#include <string_view>
#include <vector>

namespace maxStateMachine {

	template<typename T>
	class EnumValue {
	public:

		constexpr explicit EnumValue(T value, std::string_view representation) noexcept;

		T value_;
		std::string_view representation_;

	};

	template<typename T>
	class EnumMatcher {
	public:

		constexpr explicit EnumMatcher(std::vector<EnumValue<T>> values) noexcept;

		//constexpr bool DoesMatch(const Input& input) noexcept;

		std::vector<EnumValue<T>> values_;

	};

} // namespace maxStateMachine

#include <maxStateMachine/EnumMatcher.inl>

#endif // #ifndef MAXSTATEMACHINE_ENUMMATCHER_HPP