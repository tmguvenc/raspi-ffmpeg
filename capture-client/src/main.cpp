#include <zmq.h>
#include <assert.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "NewDecoder.h"
#include <tbb/concurrent_queue.h>
#include <tbb/tbb_thread.h>
#include <memory>
#include <malloc.h>

extern "C"
{
#include "./libavformat/avformat.h"
#include "./libavcodec/avcodec.h"
#include "./libavdevice/avdevice.h"
#include "./libswscale/swscale.h"
#include "./libavutil/imgutils.h"
}

#define WIDTH 640
#define HEIGHT 480

class Frame
{
public:
	Frame() : m_size(0), m_data(nullptr){}

	Frame(void* data, size_t size, size_t index) : m_size(size), m_index(index)
	{
		m_data = malloc(size);
		memcpy(m_data, data, size);
	}
	~Frame()
	{
		if (m_data)
		{
			free(m_data);
			m_size = 0;
		}
	}

	size_t m_size;
	size_t m_index;
	void* m_data;
};

typedef std::shared_ptr<Frame> spFrame;

int main()
{
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();

	auto ctx = zmq_ctx_new();

	assert(ctx);

	auto socket = zmq_socket(ctx, ZMQ_REQ);

	assert(socket);

	tbb::concurrent_bounded_queue<spFrame> queue;

	//zmq_connect(socket, "tcp://192.168.200.58:5555");
	zmq_connect(socket, "tcp://192.168.1.25:5555");
	//zmq_connect(socket, "tcp://176.234.167.53:5555");

	tbb::tbb_thread *thread = new tbb::tbb_thread([](tbb::concurrent_bounded_queue<spFrame>* q)
	{
		NewDecoder decoder;
		decoder.setup(AV_CODEC_ID_MJPEG);

		const auto len = HEIGHT * WIDTH * 3;
		char buffer[len] = { 0 };

		cv::namedWindow("image");

		while (true)
		{
			spFrame frame;
			q->pop(frame);

			if (frame->m_size == 0) break;

			decoder.decode(frame->m_data, frame->m_size, buffer, len);

			cv::Mat image(HEIGHT, WIDTH, CV_8UC3, buffer);

			cv::putText(image, std::to_string(frame->m_index), cv::Point(10, image.rows - 10), CV_FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 1, CV_AA);

			cv::imshow("image", image);

			if (cv::waitKey(1) == 0) break;
		}

	}, &queue);

	const auto size = WIDTH * HEIGHT * 3;
	auto frame_buffer = static_cast<char*>(malloc(size));

	int index = 0;
	while (true)
	{
		zmq_send(socket, "frame", 5, 0);
		auto recBytes = zmq_recv(socket, frame_buffer, size, 0);
		
		queue.push(std::make_shared<Frame>(frame_buffer, recBytes, ++index));
	}

	delete[] frame_buffer;

	zmq_close(socket);
	zmq_ctx_destroy(ctx);
}