#include <maxStateMachine/NodeTest.hpp>

#include <utility>

#include <maxStateMachine/Node.hpp>
#include <maxStateMachine/Transition.hpp>
#include <maxStateMachine/RangeMatcher.hpp>
#include <maxStateMachine/StringMatcher.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunNodeTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto NodeTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::Node test suite", std::move( ResultPolicy ) };

		NodeTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "Node transitions match correct match", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool range_callback_called = false;
			auto range_callback = [&range_callback_called](const uint32_t& input) {
				range_callback_called = true;
				return size_t{2};
			};

			auto range_transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(range_callback)};

			bool string_callback_called = false;
			auto string_callback = [&string_callback_called](const std::string_view& input) {
				string_callback_called = true;
				return size_t{3};
			};

			auto string_transition = maxStateMachine::Transition{maxStateMachine::StringMatcher{std::string_view{"test"}}, std::move(string_callback)};

			auto node = maxStateMachine::MakeNode(std::move(range_transition), std::move(string_transition));

			auto new_node_index = node.AttemptTransition(1);

			CurrentTest.MAX_TESTING_ASSERT( new_node_index == 2 );
			CurrentTest.MAX_TESTING_ASSERT( range_callback_called );
			CurrentTest.MAX_TESTING_ASSERT( !string_callback_called );
			}
		} );

		NodeTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "Node transitions doesn't erronorously match", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool range_callback_called = false;
			auto range_callback = [&range_callback_called](const uint32_t& input) {
				range_callback_called = true;
				return 2;
			};

			auto range_transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(range_callback)};

			bool string_callback_called = false;
			auto string_callback = [&string_callback_called](const std::string_view& input) {
				string_callback_called = true;
				return 3;
			};

			auto string_transition = maxStateMachine::Transition{maxStateMachine::StringMatcher{std::string_view{"test"}}, std::move(string_callback)};

			auto node = maxStateMachine::MakeNode(std::move(range_transition), std::move(string_transition));

			auto new_node_index = node.AttemptTransition(2);

			CurrentTest.MAX_TESTING_ASSERT( !new_node_index );
			CurrentTest.MAX_TESTING_ASSERT( !range_callback_called );
			CurrentTest.MAX_TESTING_ASSERT( !string_callback_called );
			}
		} );

		NodeTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "Node transitions matches correct match (2)", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			bool range_callback_called = false;
			auto range_callback = [&range_callback_called](const uint32_t& input) {
				range_callback_called = true;
				return 2;
			};

			auto range_transition = maxStateMachine::Transition{maxStateMachine::RangeMatcher{0, 1}, std::move(range_callback)};

			bool string_callback_called = false;
			auto string_callback = [&string_callback_called](const std::string_view& input) {
				string_callback_called = true;
				return 3;
			};

			auto string_transition = maxStateMachine::Transition{maxStateMachine::StringMatcher{std::string_view{"test"}}, std::move(string_callback)};

			auto node = maxStateMachine::MakeNode(std::move(range_transition), std::move(string_transition));

			auto new_node_index = node.AttemptTransition(std::string_view{"test"});

			CurrentTest.MAX_TESTING_ASSERT( new_node_index == 3 );
			CurrentTest.MAX_TESTING_ASSERT( !range_callback_called );
			CurrentTest.MAX_TESTING_ASSERT( string_callback_called );
			}
		} );


		NodeTestSuite.RunTests();
	}

} // namespace maxStateMachine