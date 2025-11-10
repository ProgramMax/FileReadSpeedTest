#include <maxStateMachine/StringMatcherTest.hpp>

#include <maxStateMachine/StringInput.hpp>
#include <maxStateMachine/StringMatcher.hpp>

#include <max/Testing/CoutResultPolicy.hpp>
#include <max/Testing/TestSuite.hpp>

namespace maxStateMachine {

	void RunStringMatcherTestSuite() noexcept {

		max::Testing::CoutResultPolicy ResultPolicy;
		auto StringMatcherTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxStateMathinc::StringMatcher test suite", std::move( ResultPolicy ) };

		StringMatcherTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor initializes members", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			constexpr auto string_matcher = maxStateMachine::StringMatcher{std::string_view{"test"}};

			static_assert( string_matcher.value_ == "test", "constructor should set value" );
			}
		} );

		StringMatcherTestSuite.AddTest( max::Testing::Test< max::Testing::CoutResultPolicy >{ "DoesMatch() matches value", []( max::Testing::Test< max::Testing::CoutResultPolicy > & CurrentTest, max::Testing::CoutResultPolicy const & ResultPolicy ) {
			constexpr auto string_matcher = maxStateMachine::StringMatcher{std::string_view{"test"}};

			static_assert( !string_matcher.DoesMatch("not test"), "DoesMatch() returns false on non-match" );
			static_assert(  string_matcher.DoesMatch("test"), "DoesMatch() returns true on match" );
			}
		} );

		StringMatcherTestSuite.RunTests();
	}

} // namespace maxStateMachine