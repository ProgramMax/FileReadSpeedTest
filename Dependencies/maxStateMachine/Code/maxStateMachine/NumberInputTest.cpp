#include <maxStateMachine/NumberInputTest.hpp>

#include <utility>

#include <maxStateMachine/NumberInput.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunNumberInputTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto NumberInputTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::NumberInput test suite", std::move( ResultPolicy ) };

		NumberInputTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor sets members", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			constexpr auto number_input = maxStateMachine::NumberInput{1};

			static_assert( number_input.value_ == 1, "constructor should set the value" );
			}
		} );

		NumberInputTestSuite.RunTests();
	}

} // namespace maxStateMachine