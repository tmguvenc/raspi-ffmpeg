/*
 * VAFrameContainer.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VAFRAMECONTAINER_H_
#define VAFRAMECONTAINER_H_

#include <stdlib.h>

class VAFrameContainer
{
public:
	virtual ~VAFrameContainer()
	{
	}
	virtual size_t getEncodedLength() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getChannels() = 0;
	virtual size_t getFrameIndex() = 0;
	virtual size_t getFrameCount() = 0;
	virtual bool isKey() = 0;
	virtual bool decodeFrame(void*, size_t) = 0;
};

#endif /* VAFRAMECONTAINER_H_ */
