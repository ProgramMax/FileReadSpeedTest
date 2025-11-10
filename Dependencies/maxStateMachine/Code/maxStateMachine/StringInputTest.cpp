#include <maxStateMachine/StringInputTest.hpp>

#include <utility>

#include <maxStateMachine/StringInput.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunStringInputTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto StringInputTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::StringInput test suite", std::move( ResultPolicy ) };

		StringInputTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor sets members", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			constexpr auto string_input = maxStateMachine::StringInput{std::string_view{"test"}};

			static_assert( string_input.value_ == "test", "constructor should set value" );
			}
		} );

		StringInputTestSuite.RunTests();
	}

} // namespace maxStateMachine