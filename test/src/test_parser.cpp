#include <parser.h>
#include <gtest/gtest.h>

struct ArgumentParserTest : testing::Test
{
	ArgumentParser *parser;

	ArgumentParserTest()
	{
		parser = new ArgumentParser;
	}
	virtual ~ArgumentParserTest()
	{
		delete parser;
	}
};

TEST_F(ArgumentParserTest, ThrowsWhenInvalidCodec)
{
	char *argv[] = { "capture-send", "-c", "asd" };
	ASSERT_THROW(parser->parse(3, argv), std::invalid_argument);
}

TEST_F(ArgumentParserTest, ThrowsWhenInvalidFPS)
{
	char *argv[] = { "capture-send", "-f", "asd" };
	ASSERT_THROW(parser->parse(3, argv), std::invalid_argument);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}