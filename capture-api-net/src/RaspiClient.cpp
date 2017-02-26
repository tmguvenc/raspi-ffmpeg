#include "RaspiClient.h"
#include "connector.h"
#include <assert.h>

using namespace Client;

RaspiClient::RaspiClient(System::Windows::Forms::Control^ control, System::String^ ip, System::UInt16 port) :
m_control(control),
m_started(false),
m_initialized(false),
m_frame_queue(new tbb::concurrent_bounded_queue<spFrame>) {

	m_connector = new Connector(ManagedtoNativeString("tcp://" + ip + ":" + System::Convert::ToString(port)), m_frame_queue);
	m_destWidth = m_connector->getWidth();
	m_destHeight = m_connector->getHeight();
	m_decoder = new Decoder(m_destWidth, m_destHeight);
	m_decoder->setup(static_cast<AVCodecID>(m_connector->getCodec()), AV_PIX_FMT_YUV420P);

	m_graphics = m_control->CreateGraphics();
	m_bmp = gcnew System::Drawing::Bitmap(m_destWidth, m_destHeight);
}

RaspiClient::~RaspiClient()
{
	this->!RaspiClient();
}

RaspiClient::!RaspiClient() {
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

	if (m_connector) {
		m_connector->stop();
	}

	m_frame_queue->push(spFrame(nullptr));
	m_decoder_thread->Join();
	m_receiver_thread->Join();
	m_frame_queue->clear();
	m_receiver_thread->~Thread(); 
	m_decoder_thread->~Thread();
	delete m_decoder_thread;
	m_decoder_thread = nullptr;
	delete m_receiver_thread;
	m_receiver_thread = nullptr;
}

void RaspiClient::decode_loop()
{
	const auto len = m_destWidth * m_destHeight * 3;
	const System::Drawing::Rectangle roi = System::Drawing::Rectangle(0, 0, m_destWidth, m_destHeight);

	while (m_started)
	{
		spFrame frame;
		m_frame_queue->pop(frame);
		if (!frame || frame->m_size == 0)
			break;

		System::Drawing::Imaging::BitmapData^ bmpData = m_bmp->LockBits(roi, System::Drawing::Imaging::ImageLockMode::ReadWrite, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		auto decoded = m_decoder->decode(frame->m_data, frame->m_size, bmpData->Scan0.ToPointer(), len);
		m_bmp->UnlockBits(bmpData);

		if (decoded)
			m_graphics->DrawImage(m_bmp, roi);

		frame.reset();
	}
}

void RaspiClient::receive_loop()
{
	if (m_connector) {
		m_connector->start();
	}
}