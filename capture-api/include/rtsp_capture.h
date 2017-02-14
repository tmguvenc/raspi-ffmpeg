/*
 * RTSPCapture.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef RTSPCAPTURE_H_
#define RTSPCAPTURE_H_

#include <string>
#include <video_file_capture.h>

class RTSPCapture: public VideoFileCapture {
	friend class RTSPCaptureFactory;
public:
	RTSPCapture(const RTSPCapture&) = delete;
	RTSPCapture& operator=(const RTSPCapture&) = delete;
	virtual ~RTSPCapture();

protected:
	explicit RTSPCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger);
};

#endif /* RTSPCAPTURE_H_ */
