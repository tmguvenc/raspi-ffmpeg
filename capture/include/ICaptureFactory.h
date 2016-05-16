/*
 * ICaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTUREFACTORY_H_
#define ICAPTUREFACTORY_H_

#include <string>

class ICapture;

class ICaptureFactory {
public:
	virtual ~ICaptureFactory() {

	}
	virtual ICapture* createFactory(const std::string& connectionString) = 0;
};

#endif /* ICAPTUREFACTORY_H_ */
