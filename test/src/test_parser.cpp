#include <parser.h>
#include <gtest/gtest.h>
#include <unordered_map>
#include <ostream>

struct ParserState
{
	std::string opt_name;
	std::string opt_value;
	bool throws;

	friend std::ostream& operator<<(std::ostream& os, const ParserState& obj)
	{
		return os
			<< "opt_name: " << obj.opt_name
			<< " opt_value: " << obj.opt_value
			<< " throws: " << obj.throws;
	}
};

struct ArgumentParserTestAll : testing::TestWithParam<ParserState>
{
	ArgumentParser *parser;

	ArgumentParserTestAll()
	{
		parser = new ArgumentParser;
	}
	virtual ~ArgumentParserTestAll()
	{
		delete parser;
	}
};

TEST_P(ArgumentParserTestAll, OptionValidation)
{
	auto parser_state = GetParam();
	char *argv[] = { "capture-send", const_cast<char*>(parser_state.opt_name.c_str()), const_cast<char*>(parser_state.opt_value.c_str()) };
	if (parser_state.throws)
		ASSERT_THROW(parser->parse(3, argv), std::invalid_argument);
	else
		ASSERT_NO_THROW(parser->parse(3, argv), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(AllInvalid, ArgumentParserTestAll, testing::Values(
	ParserState{ "-c", "h265", true },
	ParserState{ "-f", "asd", true },
	ParserState{ "-u", "", true },
	ParserState{ "-a", "asdw", true },
	ParserState{ "-a", "", true },
	ParserState{ "-r", "1280*960", true },
	ParserState{ "-c", "h264", false },
	ParserState{ "-c", "mjpeg", false },
	ParserState{ "-c", "raw", false },
	ParserState{ "-c", "-raw", true },
	ParserState{ "-c", "", true },
	ParserState{ "-r", "1280x720", false },
	ParserState{ "-r", "1920x-1080", true },
	ParserState{ "-r", "asdxbnm", true },
	ParserState{ "-r", "-1920xasd", true },
	ParserState{ "-r", "asdx1080", true },
	ParserState{ "-r", "", true },
	ParserState{ "-f", "1234", true },
	ParserState{ "-f", "", true },
	ParserState{ "-f", "*0", true },
	ParserState{ "-f", "0*", true },
	ParserState{ "-f", "-1", true },
	ParserState{ "-b", "-10", true },
	ParserState{ "-b", "5", false },
	ParserState{ "-b", "", true },
	ParserState{ "-u", "/dev/video0", false },
	ParserState{ "-u", "/dev/video1", false },
	ParserState{ "-p", "asd", true },
	ParserState{ "-p", "1234", false },
	ParserState{ "-p", "1023", true },
	ParserState{ "-p", "65537", true },
	ParserState{ "-p", "-10", true }
));

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}