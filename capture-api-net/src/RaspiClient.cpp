#include "RaspiClient.h"
#include "connector.h"

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

	m_frame_queue = new tbb::concurrent_bounded_queue<spFrame>;

	m_connector = new Connector(ManagedtoNativeString(serverUrl), m_frame_queue, m_destWidth, m_destHeight);

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

	if (m_connector) {
		delete m_connector;
		m_connector = nullptr;
	}

	if (m_frame_queue){
		delete m_frame_queue;
		m_frame_queue = nullptr;
	}

	if (m_bmp){
		m_bmp->~Bitmap();
		delete m_bmp;
		m_bmp = nullptr;
	}
}

#undef PixelFormat

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

	if (m_connector) {
		m_connector->stop();
	}
}

void RaspiClient::decode_loop()
{
	const auto len = m_destWidth * m_destHeight * 3;

	while (m_started)
	{
		spFrame frame;
		m_frame_queue->pop(frame);
		if (!frame || frame->m_size == 0)
			break;
		
		System::Drawing::Imaging::BitmapData^ bmpData = m_bmp->LockBits(System::Drawing::Rectangle(0, 0, m_destWidth, m_destHeight), System::Drawing::Imaging::ImageLockMode::ReadWrite, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		auto decoded = m_decoder->decode(frame->m_data, frame->m_size, bmpData->Scan0.ToPointer(), len);
		m_bmp->UnlockBits(bmpData);

		if (decoded)
			m_graphics->DrawImage(m_bmp, System::Drawing::Rectangle(0, 0, m_control->Width, m_control->Height));

		frame.reset();
	}
}

void RaspiClient::receive_loop()
{
	if (m_connector) {
		m_connector->start();
	}
}