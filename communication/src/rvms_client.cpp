#include <rvms_client.h>
#include <sensor_data.h>
#include <video_frame.h>

extern "C"
{
#include "./libavformat/avformat.h"
#include "./libavcodec/avcodec.h"
#include "./libavdevice/avdevice.h"
#include "./libswscale/swscale.h"
#include "./libavutil/imgutils.h"
}

RvmsClient::RvmsClient(const std::string& ip, int port) :
m_started(false),
m_port(port),
m_ip(std::move(ip)),
m_frame_queue(new tbb::concurrent_bounded_queue<Data*>)
{
	m_receiveStrategy = new ReceiveStrategyQueue<tbb::concurrent_bounded_queue<Data*>>(m_frame_queue);
	m_connector = new Connector("tcp://" + ip + ":" + std::to_string(port), m_receiveStrategy);
	m_width = m_connector->getWidth();
	m_height = m_connector->getHeight();
	m_videoDecoder = new Decoder(m_width, m_height);
	m_videoDecoder->setup(static_cast<AVCodecID>(m_connector->getCodec()), AV_PIX_FMT_YUV420P);
}

RvmsClient::~RvmsClient()
{
	if (m_connector)
	{
		m_connector->stop();
		delete m_connector;
		m_connector = nullptr;
	}

	if (m_videoDecoder)
	{
		m_videoDecoder->teardown();
		delete m_videoDecoder;
		m_videoDecoder = nullptr;
	}

	if (m_receiveStrategy)
	{
		delete m_receiveStrategy;
		m_receiveStrategy = nullptr;
	}

	if (m_frame_queue)
	{
		delete m_frame_queue;
		m_frame_queue = nullptr;
	}
}

void RvmsClient::start(char *BYTE)
{
	if (m_started.load() == true) return;

	m_buffer = BYTE;

	m_receive_thread = std::make_unique<tbb::tbb_thread>([this]() { receive_loop(); });
	m_decode_thread = std::make_unique<tbb::tbb_thread>([this]() { decode_loop(); });

	m_started.store(true);
}

void RvmsClient::stop()
{
	if (m_started.load() == false) return;

	m_connector->stop();
	if (m_decode_thread->joinable())
		m_decode_thread->join();
	if (m_receive_thread->joinable())
		m_receive_thread->join();

	m_started.store(false);
}

void RvmsClient::initialize()
{
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
}

void RvmsClient::readSensor()
{
	m_connector->readSensor();
}

void RvmsClient::moveRight()
{
	m_connector->moveRight();
}

void RvmsClient::moveUp()
{
	m_connector->moveUp();
}

void RvmsClient::moveDown()
{
	m_connector->moveDown();
}

void RvmsClient::moveLeft()
{
	m_connector->moveLeft();
}

void RvmsClient::receive_loop()
{
	m_connector->start();
}

void RvmsClient::decode_loop()
{
	const auto len = m_width * m_height * 3;

	while (m_started)
	{
		Data* data;
		m_frame_queue->pop(data);
		if (!data)
			break;

		switch (data->type())
		{
		case 1:
		{
			auto frame = static_cast<VideoFrame*>(data);

			if (!frame || frame->getSize() == 0)
				break;

			//System::Drawing::Imaging::BitmapData^ bmpData = m_bmp->LockBits(roi, System::Drawing::Imaging::ImageLockMode::ReadWrite, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			auto decoded = m_videoDecoder->decode(frame->getData(), frame->getSize(), m_buffer, len);
			//m_bmp->UnlockBits(bmpData);

			//if (decoded)
			//	m_graphics->DrawImage(m_bmp, roi);
			delete frame;
		}break;
		case 2:
		{
			auto sensorData = static_cast<HumTempSensorData*>(data);

			auto h = sensorData->getHumidity();
			auto t = sensorData->getTemperature();

			delete sensorData;
		}break;
		}
	}
}