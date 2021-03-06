/*
 * RTSPCapture.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef RTSPCAPTURE_H_
#define RTSPCAPTURE_H_

#include "VideoFileCapture.h"

class RTSPCapture: public VideoFileCapture {
	friend class RTSPCaptureFactory;
public:
	RTSPCapture(const RTSPCapture&) = delete;
	RTSPCapture& operator=(const RTSPCapture&) = delete;
	virtual ~RTSPCapture();

protected:
	explicit RTSPCapture(const std::string& connectionString);
};

#endif /* RTSPCAPTURE_H_ */
