#ifndef WEBCAM_WRAPPER_H
#define WEBCAM_WRAPPER_H

#include <string>
#include <webcam_capture_factory.h>
#include <memory>
#include <capture_base.h>
#include <vector>

class Webcam
{
public:
	explicit Webcam(const std::string& url, int width, int height, int codec, int fps);
	~Webcam();

	std::vector<unsigned char> getFrame();

private:
	WebcamCaptureFactory m_factory;
	std::unique_ptr<ICapture, CaptureStopper<ICapture>> m_capture;

};

#endif // !WEBCAM_WRAPPER_H