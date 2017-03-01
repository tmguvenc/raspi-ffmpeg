#include <parser.h>
#include <gtest/gtest.h>
#include <unordered_map>

typedef std::pair<std::string, std::string> Option;

struct ArgumentParserTestAll : testing::TestWithParam<Option>
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

TEST_P(ArgumentParserTestAll, InvalidOptions)
{
	auto option = GetParam();
	char *argv[] = { "capture-send", const_cast<char*>(option.first.c_str()), const_cast<char*>(option.second.c_str()) };
	ASSERT_THROW(parser->parse(3, argv), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Default, ArgumentParserTestAll, testing::Values(
	std::make_pair("-c", "h265"),
	std::make_pair("-f", "asd"),
	std::make_pair("-u", ""),
	std::make_pair("-a", "asdw")
));

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}