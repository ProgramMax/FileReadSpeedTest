#include <maxStateMachine/TransitionTest.hpp>

#include <utility>

#include <maxStateMachine/Transition.hpp>
#include <maxStateMachine/RangeMatcher.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunTransitionTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto TransitionTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::Transition test suite", std::move( ResultPolicy ) };

		TransitionTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "A non-match does not transition", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool callback_called = false;
			auto callback = [&callback_called](const uint32_t& input) {
				callback_called = true;
				return 2;
			};

			auto transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(callback)};

			auto new_node_index = transition.AttemptTransition(2);

			// TODO: This should be constexpr, right??
			//static_assert( !callback_called, "" );
			CurrentTest.MAX_TESTING_ASSERT( !callback_called );
			CurrentTest.MAX_TESTING_ASSERT( !new_node_index );
			}
		} );

		TransitionTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "A match transitions", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool callback_called = false;
			auto callback = [&callback_called](const uint32_t& input) {
				callback_called = true;
				return 2;
			};

			auto transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(callback)};

			auto new_node_index = transition.AttemptTransition(1);

			CurrentTest.MAX_TESTING_ASSERT( callback_called );
			CurrentTest.MAX_TESTING_ASSERT( new_node_index == 2 );
			}
		} );

		TransitionTestSuite.RunTests();
	}

} // namespace maxStateMachine