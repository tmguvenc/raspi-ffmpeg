/*
 * ICaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTUREFACTORY_H_
#define ICAPTUREFACTORY_H_

#include <capture_macros.h>
#include <string>
#include <memory>

class ICapture;

namespace spdlog
{
	class logger;
}

class CAPTURE_EXPORT ICaptureFactory {
public:
	virtual ~ICaptureFactory() { }
	virtual ICapture* create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger) = 0;
};

#endif /* ICAPTUREFACTORY_H_ */
