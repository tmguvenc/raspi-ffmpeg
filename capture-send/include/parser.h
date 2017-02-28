#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <libavcodec/avcodec.h>
#include <spdlog/spdlog.h>

struct Arguments
{
	int port;
	int fps;
	uint32_t width;
	uint32_t height;
	int balance;
	AVCodecID codec;
	std::string url;
};

class ArgumentParser
{
public:
	ArgumentParser();
	virtual ~ArgumentParser();

	Arguments parse(int argc, char* argv[]);

protected:
	std::pair<uint32_t, uint32_t> getWidthAndHeight();

private:
	std::unordered_map<std::string, std::string> m_options;
	std::shared_ptr<spdlog::logger> m_logger;
};

#endif
