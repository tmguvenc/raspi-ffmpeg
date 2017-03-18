#include <parser.h>
#include <common_utils.h>
#include <limits>
#include <vector>
#include <libavcodec/avcodec.h>
#include <iostream>
#include <fstream>

#undef max
#undef min

#define STRINGIFY(aa) #aa

template<typename T>
inline T toNumber(const char* str, T lowerBound = std::numeric_limits<T>::min(), T upperBound = std::numeric_limits<T>::max()) {
	T number;
	if (is_number(str))
		number = static_cast<T>(atoi(str));
	else
		throw std::invalid_argument(string_format("invalid argument: %s", str));

	if (number < lowerBound || number > upperBound){
		std::cout << number << " is greater than " << upperBound << " or lower than " << lowerBound << std::endl;
		throw std::out_of_range(string_format("Number is out of range"));
	}

	return number;
}

inline int getCodec(const char* codec_name) {
	if (strncmp(codec_name, "mjpeg", 5) == 0)
		return  AV_CODEC_ID_MJPEG;
	if (strncmp(codec_name, "h264", 4) == 0)
		return AV_CODEC_ID_H264;
	if (strncmp(codec_name, "raw", 3) == 0)
		return AV_CODEC_ID_RAWVIDEO;

	throw std::invalid_argument(string_format("invalid codec: %s", codec_name));
}

template<typename T>
inline std::vector<T> getPins(const char* str) {
	std::vector<T> pins;
	auto pins_str = split(str, ',');

	try {
		for (const auto pin : pins_str)
			pins.push_back(toNumber<T>(pin.c_str()));
	} catch (const std::invalid_argument& ex) { throw ex; }

	return pins;
}

ArgumentParser::ArgumentParser()
{
	// set initial values of the map
	m_options["-port"] = "5555";
	m_options["-vsize"] = "20";
	m_options["-asize"] = "20";
	m_options["-vurl"] = "/dev/video0";
	m_options["-aurl"] = "/dev/video0";
	m_options["-res"] = "640x480";
	m_options["-codec"] = "mjpeg";
	m_options["-fps"] = "15";
	m_options["-ppins"] = "0,2,3,4";
	m_options["-tpins"] = "0,2,3,4";
	m_options["-spin"] = "7";
}

ArgumentParser::~ArgumentParser()
{
}

ApplicationParams ArgumentParser::parse(int argc, char* argv[])
{
	for (auto i = 1; i < argc; ++i)
	{
		auto op = m_options.find(argv[i]);
		if (op != m_options.end())
			op->second = argv[i + 1];
		else{
			if (std::string(argv[i]).find("-") != std::string::npos){
				throw std::invalid_argument(string_format("invalid option: %s", argv[i]));
			}
		}
	}
	try{
		return getArgs();
	}
	catch (const std::invalid_argument& ex) { throw ex; }
}

ApplicationParams ArgumentParser::parse(const std::string& fileName)
{
	std::ifstream in(fileName.c_str(), std::ios::binary);
	if (!in.is_open()) {
		throw std::invalid_argument(string_format("Cannot open: %s", fileName.c_str()));
	}

	std::string line;
	while (std::getline(in, line)){
		const auto& kv = split(line, '=');
		m_options[kv[0]] = kv[1];
	}

	in.close();

	try{
		return getArgs();
	}
	catch (const std::invalid_argument& ex) { throw ex; }
}

std::pair<uint32_t, uint32_t> ArgumentParser::getWidthAndHeight()
{
	auto res = split(m_options["-res"], 'x');
	if (res.size() != 2 || !is_number(res[0]) || !is_number(res[1]))
		return std::make_pair(-1, -1);

	return std::make_pair(atoi(res[0].c_str()), atoi(res[1].c_str()));
}

ApplicationParams ArgumentParser::getArgs()
{
	ApplicationParams args;

	// get video frame buffer size
	args.max_videoframe_queue_size = toNumber<uint16_t>(m_options["-vsize"].c_str());

	// get audio frame buffer size
	args.max_audioframe_queue_size = toNumber<uint16_t>(m_options["-asize"].c_str());

	// get port to listen
	args.port = toNumber<uint16_t>(m_options["-port"].c_str(), 1025);

	// get FPS
	args.fps = toNumber<uint8_t>(m_options["-fps"].c_str(), 1, 40);

	// get video source URL
	args.video_source_url = m_options["-vurl"];
	if (args.video_source_url.empty() || args.video_source_url == " " || args.video_source_url == "") {
		throw std::invalid_argument(string_format("invalid video source URL: %s", args.video_source_url.c_str()));
	}

	// get audio source URL
	args.audio_source_url = m_options["-aurl"];
	if (args.audio_source_url.empty() || args.audio_source_url == " " || args.audio_source_url == "") {
		throw std::invalid_argument(string_format("invalid audio source URL: %s", args.audio_source_url.c_str()));
	}

	// get resolution
	auto wh = getWidthAndHeight();
	if (wh.first == -1 || wh.second == -1) {
		throw std::invalid_argument(string_format("invalid resolution: %s", m_options["-res"].c_str()));
	}

	args.width = wh.first;
	args.height = wh.second;

	// get codec
	args.codec = getCodec(m_options["-codec"].c_str());

	// get pan motor connection pins
	args.panMotorPins = std::move(getPins<uint8_t>(m_options["-ppins"].c_str()));

	// get tilt motor connection pins
	args.tiltMotorPins = std::move(getPins<uint8_t>(m_options["-tpins"].c_str()));

	// get humidity and temperature sensor pin
	args.sensorPin = toNumber<uint8_t>(m_options["-spin"].c_str());

	return args;
}