#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <common_macros.h>
#include <application_params.h>

class COMMON_EXPORT ArgumentParser
{
public:
	ArgumentParser();
	virtual ~ArgumentParser();

	ApplicationParams parse(int argc, char* argv[]);

protected:
	std::pair<uint32_t, uint32_t> getWidthAndHeight();

private:
	std::unordered_map<std::string, std::string> m_options;
};

#endif
