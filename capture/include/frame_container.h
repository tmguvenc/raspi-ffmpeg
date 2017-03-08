/*
 * frame_container.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef FRAME_CONTAINER_H
#define FRAME_CONTAINER_H

#include <capture_macros.h>
#include <stdint.h>

class CAPTURE_EXPORT FrameContainer {
public:
	virtual ~FrameContainer() {
	}
	virtual uint64_t getEncodedLength() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getChannels() = 0;
	virtual uint64_t getFrameIndex() = 0;
	virtual uint64_t getFrameCount() = 0;
	virtual bool isKey() = 0;
	virtual bool decodeFrame(void*, uint64_t) = 0;
	virtual void* data() = 0;
	virtual uint64_t size() = 0;
};

#endif /* VAFRAMECONTAINER_H_ */
