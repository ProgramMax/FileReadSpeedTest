#include <maxStateMachine/StateMachineTest.hpp>

#include <utility>

#include <maxStateMachine/Node.hpp>
#include <maxStateMachine/Transition.hpp>
#include <maxStateMachine/RangeMatcher.hpp>
#include <maxStateMachine/StateMachine.hpp>
#include <maxStateMachine/StringMatcher.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunStateMachineTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto StateMachineTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::StateMachine test suite", std::move( ResultPolicy ) };

		StateMachineTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "test name", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool range_callback_called = false;
			auto range_callback = [&range_callback_called](const uint32_t& input) {
				range_callback_called = true;
				return size_t{1};
			};
			auto range_transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(range_callback)};
			auto node_0 = maxStateMachine::MakeNode(std::move(range_transition));

			bool string_callback_called = false;
			auto string_callback = [&string_callback_called](const std::string_view& input) {
				string_callback_called = true;
				return size_t{0};
			};
			auto string_transition = maxStateMachine::Transition{maxStateMachine::StringMatcher{std::string_view{"test"}}, std::move(string_callback)};
			auto node_1 = maxStateMachine::MakeNode(std::move(string_transition));

			auto state_machine = maxStateMachine::StateMachine{std::make_tuple(std::move(node_0), std::move(node_1))};

			state_machine.AttemptTransition(1);

			CurrentTest.MAX_TESTING_ASSERT( state_machine.current_node_index_ == 1 );
			CurrentTest.MAX_TESTING_ASSERT( range_callback_called );
			CurrentTest.MAX_TESTING_ASSERT( !string_callback_called );

			// reset
			range_callback_called = false;

			state_machine.AttemptTransition(std::string_view{"test"});

			CurrentTest.MAX_TESTING_ASSERT( state_machine.current_node_index_ == 0 );
			CurrentTest.MAX_TESTING_ASSERT( !range_callback_called );
			CurrentTest.MAX_TESTING_ASSERT( string_callback_called );

		}
		} );

		StateMachineTestSuite.RunTests();
	}

} // namespace maxStateMachine