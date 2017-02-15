#include "RaspiClient.h"

using namespace Client;

RaspiClient::RaspiClient(System::Windows::Forms::Control^ control, System::String^ serverUrl, System::Int32 destWidth, System::Int32 destHeight, CodecType codec) :
m_destWidth(destWidth),
m_destHeight(destHeight),
m_control(control),
m_started(false),
m_initialized(false) {
	m_decoder = new Decoder(m_destWidth, m_destHeight);
	AVCodecID c = codec == CodecType::MJPEG ? AV_CODEC_ID_MJPEG : codec == CodecType::H264 ? AV_CODEC_ID_H264 : AV_CODEC_ID_RAWVIDEO;
	m_decoder->setup(c, AV_PIX_FMT_YUV420P);
	m_context = zmq_ctx_new();

	m_socket = zmq_socket(m_context, ZMQ_REQ);

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.

	m_frame_queue = new tbb::concurrent_bounded_queue<spFrame>;
	m_url = new std::string(ManagedtoNativeString(serverUrl).c_str());
	m_graphics = m_control->CreateGraphics();
	m_bmp = gcnew System::Drawing::Bitmap(m_destWidth, m_destHeight);
}

RaspiClient::~RaspiClient() {
	m_graphics->~Graphics();
	delete m_graphics;
	m_graphics = nullptr;

	stop();

	if (m_decoder) {
		m_decoder->teardown();
		delete m_decoder;
		m_decoder = nullptr;
	}

	if (m_socket) {
		auto rc = zmq_disconnect(m_socket, m_url->c_str());
		rc = zmq_close(m_socket);
		m_socket = nullptr;
	}
	if (m_context) {
		auto rc = zmq_ctx_destroy(m_context);
		m_context = nullptr;
	}

	if (m_frame_queue){
		delete m_frame_queue;
		m_frame_queue = nullptr;
	}

	if (m_url) {
		delete m_url;
		m_url = nullptr;
	}

	if (m_bmp){
		m_bmp->~Bitmap();
		delete m_bmp;
		m_bmp = nullptr;
	}
}

#undef PixelFormat

void RaspiClient::ImageWrite(array<System::Byte>^ data)
{
	System::Drawing::Imaging::BitmapData^ bmpData = m_bmp->LockBits(System::Drawing::Rectangle(0, 0, m_destWidth, m_destHeight), System::Drawing::Imaging::ImageLockMode::ReadWrite, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
	System::Runtime::InteropServices::Marshal::Copy(data, 0, bmpData->Scan0, data->Length);
	m_bmp->UnlockBits(bmpData);
	m_graphics->DrawImage(m_bmp, System::Drawing::Rectangle(0, 0, m_destWidth, m_destHeight));
}

void RaspiClient::initialize() {
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
}

void RaspiClient::start()
{
	if (m_started) return;

	m_started = true;
	// "tcp://192.168.1.25:5555"
	zmq_connect(m_socket, m_url->c_str());

	m_receiver_thread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &RaspiClient::receive_loop));
	m_decoder_thread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &RaspiClient::decode_loop));

	m_receiver_thread->Start();
	m_decoder_thread->Start();
}

void RaspiClient::stop()
{
	if (!m_started) return;

	m_started = false;

	m_frame_queue->push(spFrame(nullptr));
	m_decoder_thread->Join();
	m_receiver_thread->Join();
	m_frame_queue->clear();

	zmq_send(m_socket, "stop", 4, 0);
}

void RaspiClient::decode_loop()
{
	uint32_t len = m_destWidth * m_destHeight * 3;
	array<System::Byte>^ cli_buffer = gcnew array<System::Byte>(len);
	pin_ptr<unsigned char> ptr = &cli_buffer[0];

	while (m_started)
	{
		spFrame frame;
		m_frame_queue->pop(frame);
		if (!frame || frame->m_size == 0)
			break;
		if (!m_decoder->decode(frame->m_data, frame->m_size, ptr, len))
			continue;
		ImageWrite(cli_buffer);
	}
}

void RaspiClient::receive_loop()
{
	const auto size = m_destWidth * m_destHeight * 3;
	auto frame_buffer = static_cast<char*>(malloc(size));

	int index = 0;
	while (m_started)
	{
		zmq_send(m_socket, "frame", 5, 0);
		auto recBytes = zmq_recv(m_socket, frame_buffer, size, 0);
		m_frame_queue->push(std::make_shared<Frame>(frame_buffer, recBytes, ++index));
	}

	free(frame_buffer);
}