#ifndef MAXSTATEMACHINE_STATEMACHINE_HPP
#define MAXSTATEMACHINE_STATEMACHINE_HPP

#include <tuple>
#include <utility>

namespace maxStateMachine {

	template<typename... NodeTypes>
	class StateMachine {
	public:

		constexpr explicit StateMachine(std::tuple<NodeTypes...> nodes) noexcept
			: nodes_(std::move(nodes))
			, current_node_index_(0)
		{}

		template<typename T>
		constexpr void AttemptTransition(T input) noexcept {
			auto transition_happened = false;
			auto i = size_t{0};
			auto attempt_transition = [this, &transition_happened, &i, &input](auto&& arg) {
				if (!transition_happened && current_node_index_ == i++) {
					auto possible_new_node_index = arg.AttemptTransition(input);
					if (possible_new_node_index) {
						transition_happened = true;
						current_node_index_ = std::move(possible_new_node_index.value());
					}
				}
			};

			std::apply([&attempt_transition](auto&&... args) {
				((attempt_transition(args)), ...);
			}, nodes_);
		}

		std::tuple<NodeTypes...> nodes_;
		size_t current_node_index_;

	};

} // namespace maxStateMachine

#endif // #ifndef MAXSTATEMACHINE_STATEMACHINE_HPP