#ifndef UTILS_H
#define UTILS_H

#include <chrono>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS (3 * HEARTHBEAT_INTERVAL_IN_SECONDS)
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2

inline int64_t current_time(){
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return static_cast<int64_t>(temp) / 1000;
}


#endif