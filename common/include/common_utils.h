#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <sstream>
#include <vector>
#include <cctype>

#if defined(_WIN32) && defined(_MSC_VER)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS (3 * HEARTHBEAT_INTERVAL_IN_SECONDS)
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2

inline std::string string_format(const char *fmt, ...)
{
#if defined(_WIN32) && defined(_MSC_VER)
	char buff[256] = { 0 };
	va_list args;
	va_start(args, fmt);
	auto size = vsnprintf_s(buff, _countof(buff), _TRUNCATE, fmt, args);
	va_end(args);
	return std::string(buff, size);
#else
	va_list args1;
	va_start(args1, fmt);
	std::vector<char> buf(1 + vsnprintf(nullptr, 0, fmt, args1));
	vsnprintf(buf.data(), buf.size(), fmt, args1);
	va_end(args1);
	return std::string(buf.data(), buf.size());
#endif
}

inline int64_t current_time(){
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return static_cast<int64_t>(temp) / 1000;
}

inline std::string getHostName()
{
#if defined(_WIN32) && defined(_MSC_VER)
	unsigned long len = 1024;
	char buffer[1024];
	GetComputerName(buffer, &len);
	return std::string(buffer, 0, len);
#else
	char buffer[1024] = { 0 };
	auto ret = gethostname(buffer, 1024);
	return std::string(buffer);
#endif
}

inline std::vector<std::string> split(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (std::getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

inline static bool is_number(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

template<typename Queue>
void inline clearQueue(Queue* frameQueue) {

	using T = typename Queue::value_type;

	static_assert(std::is_pointer<T>::value, "Queue requires pointer type as element.");

	while (!frameQueue->empty()) {
		T element;
		frameQueue->pop(element);
		delete element;
	}
}

#endif
