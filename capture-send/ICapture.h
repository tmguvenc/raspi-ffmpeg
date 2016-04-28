/*
 * ICapture.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTURE_H_
#define ICAPTURE_H_

class ICapture{
public:
	virtual ~ICapture() {}
	virtual void init() = 0;
	virtual void start() = 0;
	virtual void teardown() = 0;
};

#endif /* ICAPTURE_H_ */
