#include <parser.h>
#include <gtest/gtest.h>
#include <unordered_map>
#include <ostream>

struct ParserState
{
	std::unordered_map<std::string, std::string> option_map;
	bool throws;

	friend std::ostream& operator<<(std::ostream& os, const ParserState& obj)
	{
		for (const auto& p : obj.option_map)
			os << p.first << ": " << p.second << std::endl;
		os << "throws: " << obj.throws;

		return os;
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
	auto argv = new char*[parser_state.option_map.size() * 2];
	auto index = 0;
	for (const auto& p : parser_state.option_map)
	{
		auto first_len = p.first.size() + 1;
		argv[index] = new char[first_len];
		strcpy(argv[index++], p.first.c_str());

		auto second_len = p.second.size() + 1;
		argv[index] = new char[second_len];
		strcpy(argv[index++], p.second.c_str());
	}

	if (parser_state.throws)
		ASSERT_THROW(parser->parse(parser_state.option_map.size() * 2, argv), std::invalid_argument);
	else
		ASSERT_NO_THROW(parser->parse(parser_state.option_map.size() * 2, argv));

	for (auto i = 0; i < index; ++i)
		delete[] argv[i];

	delete[] argv;
}

INSTANTIATE_TEST_CASE_P(MapTest, ArgumentParserTestAll, testing::Values(
	ParserState{ { { "-c", "h264" }, { "-f", "25" }, { "-r", "1280x960" }, { "-b", "12" }, { "-u", "/dev/video1" }, { "-p", "12345" }, }, false },
	ParserState{ { { "-c", "h264" }, { "-f", "12" }, { "-r", "1280x960" }, { "-b", "12" }, { "-u", "/dev/video1" }, { "-p", "12345" }, }, false },
	ParserState{ { { "-c", "h264" }, { "-f", "12" }, { "-r", "1025" }, { "-b", "12" }, { "-u", "" }, { "-p", "12" }, }, true }
));


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
