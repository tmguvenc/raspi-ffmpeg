#include <parser.h>
#include <common_utils.h>
#include <exception>
#include <limits>

#undef max

ArgumentParser::ArgumentParser() 
{
	// set initial values of the map
	m_options["-p"] = "5555";
	m_options["-b"] = "20";
	m_options["-u"] = "/dev/video0";
	m_options["-r"] = "640x480";
	m_options["-c"] = "mjpeg";
	m_options["-f"] = "15";
}

ArgumentParser::~ArgumentParser()
{
}

Arguments ArgumentParser::parse(int argc, char* argv[])
{
	Arguments args;

	for (auto i = 1; i < argc; ++i)
	{
		auto op = m_options.find(argv[i]);
		if (op != m_options.end())
			op->second = argv[i + 1];
		else{
			if(std::string(argv[i]).find("-") != std::string::npos){
				char buffer[100] = { 0 };
				sprintf(buffer, "invalid option: %s", argv[i]);
				throw std::invalid_argument(buffer);
			}
		}
	}

	// get frame queue capacity
	if (is_number(m_options["-b"])) {
		args.balance = atoi(m_options["-b"].c_str());
	}
	else {
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid queue size: %s", m_options["-b"].c_str());
		throw std::invalid_argument(buffer);
	}

	// get port number
	if (is_number(m_options["-p"])) {
		
		auto max = std::numeric_limits<uint16_t>::max();

		args.port = atoi(m_options["-p"].c_str());
		if (args.port < 1025 || args.port > max) {
			char buffer[100] = { 0 };
			sprintf(buffer, "invalid port number (must be between 1025 and 65536): %s", m_options["-p"].c_str());
			throw std::invalid_argument(buffer);
		}
	}
	else {
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid port number: %s", m_options["-p"].c_str());
		throw std::invalid_argument(buffer);
	}

	// get source URL
	args.url = m_options["-u"];

	if (args.url.empty()) {
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid source URL: %s", m_options["-u"].c_str());
		throw std::invalid_argument(buffer);
	}

	// get resolution
	auto wh = getWidthAndHeight();

	if (wh.first == -1 || wh.second == -1) {
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid resolution: %s", m_options["-r"].c_str());
		throw std::invalid_argument(buffer);
	}

	args.width = wh.first;
	args.height = wh.second;

	// get codec
	auto codec_name = m_options["-c"];
	if (codec_name == "mjpeg")
		args.codec = AV_CODEC_ID_MJPEG;
	else if (codec_name == "h264")
		args.codec = AV_CODEC_ID_H264;
	else if (codec_name == "raw")
		args.codec = AV_CODEC_ID_RAWVIDEO;
	else
	{
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid codec: %s", m_options["-c"].c_str());
		throw std::invalid_argument(buffer);
	}

	// get FPS
	if (is_number(m_options["-f"])) {
		args.fps = atoi(m_options["-f"].c_str());
		if (args.fps < 1 || args.fps > 40) {
			char buffer[100] = { 0 };
			sprintf(buffer, "invalid fps (must be between 1 and 40): %s", m_options["-f"].c_str());
			throw std::invalid_argument(buffer);
		}
	} else {
		char buffer[100] = { 0 };
		sprintf(buffer, "invalid fps: %s", m_options["-f"].c_str());
		throw std::invalid_argument(buffer);
	}

	return args;
}

std::pair<uint32_t, uint32_t> ArgumentParser::getWidthAndHeight()
{
	auto res = split(m_options["-r"], 'x');	
	if (res.size() != 2 || !is_number(res[0]) || !is_number(res[1]))
		return std::make_pair(-1, -1);

	return std::make_pair(atoi(res[0].c_str()), atoi(res[1].c_str()));
}
